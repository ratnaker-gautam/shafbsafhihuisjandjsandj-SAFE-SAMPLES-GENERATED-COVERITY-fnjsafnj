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
        long temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
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
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row = a->data + (i * a->cols);
            const int* b_col = b->data + j;
            
            for (int k = 0; k < a->cols; k++) {
                if (a_row > a->data + (MAX_DIM * MAX_DIM - 1) || 
                    b_col > b->data + (MAX_DIM * MAX_DIM - 1)) {
                    return 0;
                }
                
                int product;
                if (__builtin_mul_overflow(*a_row, *b_col, &product)) {
                    return 0;
                }
                if (__builtin_add_overflow(sum, product, &sum)) {
                    return 0;
                }
                
                a_row++;
                b_col += b->cols;
            }
            
            *(res_ptr++) = sum;
        }
    }
    
    return 1;
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    matrix_init(result, a->rows, a->cols);
    
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    int size = a->rows * a->cols;
    
    for (int i = 0; i < size; i++) {
        if (__builtin_add_overflow(*a_ptr, *b_ptr, res_ptr)) {
            return 0;
        }
        a_ptr++;
        b_ptr++;
        res_ptr++;
    }
    
    return 1;
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    
    int rows2 = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    int cols2 = read