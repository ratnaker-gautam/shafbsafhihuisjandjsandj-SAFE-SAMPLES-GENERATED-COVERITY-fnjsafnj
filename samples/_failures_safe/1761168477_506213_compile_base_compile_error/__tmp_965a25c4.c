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
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min_val && value <= max_val) {
            return value;
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min_val, max_val);
    }
}

void matrix_init(Matrix* m) {
    m->rows = 0;
    m->cols = 0;
    memset(m->data, 0, sizeof(m->data));
}

void matrix_read(Matrix* m, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    m->rows = read_int("Rows: ", 1, MAX_ROWS);
    m->cols = read_int("Cols: ", 1, MAX_COLS);
    
    printf("Enter %d elements for matrix %s:\n", m->rows * m->cols, name);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[MATRIX_INDEX(*m, i, j)] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* m, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d ", m->data[MATRIX_INDEX(*m, i, j)]);
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
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int term1 = a->data[MATRIX_INDEX(*a, i, k)];
                int term2 = b->data[MATRIX_INDEX(*b, k, j)];
                if ((term1 > 0 && term2 > 0 && term1 > INT_MAX / term2) ||
                    (term1 > 0 && term2 < 0 && term2 < INT_MIN / term1) ||
                    (term1 < 0 && term2 > 0 && term1 < INT_MIN / term2) ||
                    (term1 < 0 && term2 < 0 && term1 < INT_MAX / term2)) {
                    return 0;
                }
                int product = term1 * term2;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            result->data[MATRIX_INDEX(*result, i, j)] = sum;
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
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int val1 = a->data[MATRIX_INDEX(*a, i, j)];
            int val2 = b->data[MATRIX_INDEX(*b, i, j)];
            if ((val2 > 0 && val1 > INT_MAX - val2) ||
                (val2 < 0 && val1 < INT_MIN - val2)) {
                return 0;
            }
            result->data[MATRIX_INDEX(*result, i, j)] = val1 + val2;
        }
    }
    return 1;
}

int main(void) {
    Matrix mat1, mat2, result;
    matrix_init(&mat1);
    matrix_init(&mat2);
    matrix_init(&result);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    matrix_read(&mat1, "A");
    matrix_read(&mat