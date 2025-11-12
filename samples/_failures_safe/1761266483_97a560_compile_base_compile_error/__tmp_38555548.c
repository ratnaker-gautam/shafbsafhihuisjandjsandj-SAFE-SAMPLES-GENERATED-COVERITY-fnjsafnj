//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int rows;
    int cols;
    int data[MATRIX_SIZE];
} Matrix;

#define MATRIX_GET(m, r, c) ((m).data[(r) * (m).cols + (c)])
#define MATRIX_SET(m, r, c, v) ((m).data[(r) * (m).cols + (c)] = (v))

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

void matrix_init(Matrix* m) {
    m->rows = 0;
    m->cols = 0;
    memset(m->data, 0, sizeof(m->data));
}

void matrix_input(Matrix* m, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    m->rows = read_int("Rows: ", 1, MAX_ROWS);
    m->cols = read_int("Cols: ", 1, MAX_COLS);
    
    printf("Enter %d elements for matrix %s:\n", m->rows * m->cols, name);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_SET(*m, i, j, read_int(prompt, INT_MIN, INT_MAX));
        }
    }
}

void matrix_print(const Matrix* m, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", MATRIX_GET(*m, i, j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)MATRIX_GET(*a, i, k) * (long long)MATRIX_GET(*b, k, j);
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                long long new_sum = (long long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    return 0;
                }
                sum = (int)new_sum;
            }
            MATRIX_SET(*result, i, j, sum);
        }
    }
    return 1;
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long long sum = (long long)MATRIX_GET(*a, i, j) + (long long)MATRIX_GET(*b, i, j);
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            MATRIX_SET(*result, i, j, (int)sum);
        }
    }
    return 1;
}

void matrix_transpose(const Matrix* m, Matrix* result) {
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            MATRIX_SET(*result, i, j, MATRIX_GET(*m, j, i));
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    matrix_init(&mat1);
    matrix_init(&mat2);
    matrix_init(&result);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");