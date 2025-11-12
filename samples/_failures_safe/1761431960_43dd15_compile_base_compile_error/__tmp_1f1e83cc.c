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
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat) {
    printf("Enter matrix elements row by row:\n");
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) {
                return;
            }
            *(ptr++) = val;
        }
    }
}

void matrix_print(const Matrix* mat) {
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr++));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (a->rows > MAX_DIM || b->cols > MAX_DIM) {
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
                if ((sum > 0 && *a_row > INT_MAX - sum) || 
                    (sum < 0 && *a_row < INT_MIN - sum)) {
                    return 0;
                }
                sum += *a_row * *(b_col + k * b->cols);
                a_row++;
            }
            
            *(res_ptr++) = sum;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dest) {
    if (src->rows > MAX_DIM || src->cols > MAX_DIM) {
        return 0;
    }
    
    matrix_init(dest, src->cols, src->rows);
    int* dest_ptr = dest->data;
    const int* src_ptr = src->data;
    
    for (int j = 0; j < src->cols; j++) {
        const int* col_ptr = src_ptr + j;
        for (int i = 0; i < src->rows; i++) {
            *(dest_ptr++) = *col_ptr;
            col_ptr += src->cols;
        }
    }
    
    return 1;
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    if (choice == -1) {
        return 1;
    }
    
    if (choice == 1) {
        printf("Matrix A:\n");
        int rows_a = read_int("Enter rows: ", 1, MAX_DIM);
        int cols_a = read_int("Enter columns: ", 1, MAX_DIM);
        if (rows_a == -1 || cols_a == -1) return 1;
        
        matrix_init(&a, rows_a, cols_a);
        matrix_input(&a);
        
        printf("Matrix B:\n");
        int rows_b = read_int("Enter rows: ", 1, MAX_DIM);
        int cols_b = read_int("Enter columns: ", 1, MAX_DIM);
        if (rows_b == -1 || cols_b == -1) return 1;
        
        matrix_init(&b, rows_b, cols_b);
        matrix_input(&b);
        
        printf("\nMatrix A:\n");
        matrix_print(&a);
        printf("\nMatrix B:\n");
        matrix_print(&b);
        
        if (matrix_multiply