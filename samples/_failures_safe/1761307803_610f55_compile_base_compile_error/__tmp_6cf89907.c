//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_ELEM(m, i, j, cols) ((m)[(i) * (cols) + (j)])

typedef struct {
    int rows;
    int cols;
    int data[MAX_ROWS * MAX_COLS];
} Matrix;

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long long_val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (long_val < min_val || long_val > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        value = (int)long_val;
        break;
    }
    
    return value;
}

void matrix_init(Matrix* mat) {
    mat->rows = 0;
    mat->cols = 0;
    memset(mat->data, 0, sizeof(mat->data));
}

int matrix_input(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    
    int rows = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    if (rows == -1) return 0;
    
    int cols = read_int("Columns: ", MIN_DIM, MAX_COLS);
    if (cols == -1) return 0;
    
    mat->rows = rows;
    mat->cols = cols;
    
    printf("Enter %d elements for matrix %s (%d x %d):\n", rows * cols, name, rows, cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            
            int elem = read_int(prompt, INT_MIN, INT_MAX);
            if (elem == -1) return 0;
            
            MATRIX_ELEM(mat->data, i, j, cols) = elem;
        }
    }
    
    return 1;
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", MATRIX_ELEM(mat->data, i, j, mat->cols));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 0;
    }
    
    if (a->rows > MAX_ROWS || b->cols > MAX_COLS) {
        printf("Result matrix would exceed maximum dimensions.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)MATRIX_ELEM(a->data, i, k, a->cols) * 
                                   (long long)MATRIX_ELEM(b->data, k, j, b->cols);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected during multiplication.\n");
                    return 0;
                }
                sum += (int)product;
                if ((sum > 0 && product > 0 && sum < 0) || 
                    (sum < 0 && product < 0 && sum > 0)) {
                    printf("Integer overflow detected during summation.\n");
                    return 0;
                }
            }
            MATRIX_ELEM(result->data, i, j, result->cols) = sum;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* result) {
    if (src->rows > MAX_COLS || src->cols > MAX_ROWS) {
        printf("Transposed matrix would exceed maximum dimensions.\n");
        return 0;
    }
    
    result->rows = src->cols;
    result->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            MATRIX_ELEM(result->data, j, i, result->cols) = 
                MATRIX_ELEM(src->data, i, j, src->cols);