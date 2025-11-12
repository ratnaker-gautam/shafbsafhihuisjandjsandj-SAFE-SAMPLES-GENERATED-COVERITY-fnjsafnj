//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

void matrix_init(Matrix* mat, int rows, int cols) {
    if (rows <= 0 || rows > MAX_DIM || cols <= 0 || cols > MAX_DIM) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d,%d]: ", i, j);
            int value = read_int(prompt, INT_MIN, INT_MAX);
            if (value == -1) {
                return;
            }
            *(ptr++) = value;
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    
    const int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr++));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    if (a->cols != b->rows) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    
    int* res_ptr = result->data;
    const int* a_ptr = a->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row_ptr = a_ptr;
            const int* b_col_ptr = b->data + j;
            
            for (int k = 0; k < a->cols; k++) {
                if (a_row_ptr > a->data + (MAX_DIM * MAX_DIM - 1) || 
                    b_col_ptr > b->data + (MAX_DIM * MAX_DIM - 1)) {
                    return 0;
                }
                
                long long product = (long long)(*a_row_ptr) * (long long)(*b_col_ptr);
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                
                long long new_sum = (long long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    return 0;
                }
                
                sum = (int)new_sum;
                a_row_ptr++;
                b_col_ptr += b->cols;
            }
            
            *(res_ptr++) = sum;
        }
        a_ptr += a->cols;
    }
    
    return 1;
}

int main() {
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("=============================================\n\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    int rows2 = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    
    if (rows1 == -1 || cols1 == -1 || rows2 == -1 || cols2 == -1) {
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Error: Columns of A (%d) must equal rows of B (%d) for multiplication.\n", cols1, rows2);
        return 1;
    }
    
    Matrix matA, matB, matResult;
    
    matrix_init(&matA, rows1, cols1);
    matrix_init(&matB,