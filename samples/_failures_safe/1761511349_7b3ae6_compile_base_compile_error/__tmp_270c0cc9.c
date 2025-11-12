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

#define MATRIX_INDEX(m, r, c) ((r) * (m).cols + (c))

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    do {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        break;
    } while (1);
    
    return (int)value;
}

void matrix_init(Matrix* m) {
    m->rows = 0;
    m->cols = 0;
    memset(m->data, 0, sizeof(m->data));
}

int matrix_input(Matrix* m, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    m->rows = read_int("Rows: ", 1, MAX_ROWS);
    if (m->rows == -1) return 0;
    m->cols = read_int("Columns: ", 1, MAX_COLS);
    if (m->cols == -1) return 0;
    
    printf("Enter %d elements for matrix %s:\n", m->rows * m->cols, name);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) return 0;
            m->data[MATRIX_INDEX(*m, i, j)] = val;
        }
    }
    return 1;
}

void matrix_print(const Matrix* m, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[MATRIX_INDEX(*m, i, j)]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[MATRIX_INDEX(*a, i, k)] * 
                              (long)b->data[MATRIX_INDEX(*b, k, j)];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    printf("Integer overflow detected during multiplication.\n");
                    return 0;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Integer overflow detected during multiplication.\n");
                return 0;
            }
            result->data[MATRIX_INDEX(*result, i, j)] = (int)sum;
        }
    }
    return 1;
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions must match for addition.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[MATRIX_INDEX(*a, i, j)] + 
                      (long)b->data[MATRIX_INDEX(*b, i, j)];
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Integer overflow detected during addition.\n");
                return 0;
            }
            result->data[MATRIX_INDEX(*result, i, j)] = (int)sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix* m, Matrix* result) {
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++)