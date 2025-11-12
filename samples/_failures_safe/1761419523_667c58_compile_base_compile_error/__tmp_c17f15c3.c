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

void matrix_init(Matrix* m, int rows, int cols) {
    m->rows = rows;
    m->cols = cols;
    memset(m->data, 0, sizeof(m->data));
}

void matrix_input(Matrix* m, const char* name) {
    printf("Enter values for matrix %s (%d x %d):\n", name, m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d,%d]: ", i, j);
            m->data[MATRIX_INDEX(*m, i, j)] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* m, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        printf("  ");
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[MATRIX_INDEX(*m, i, j)]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[MATRIX_INDEX(*a, i, k)] * 
                                   (long long)b->data[MATRIX_INDEX(*b, k, j)];
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                long long new_sum = (long long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    return 0;
                }
                sum = (int)new_sum;
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
    if (result->rows != a->rows || result->cols != a->cols) {
        return 0;
    }
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long long sum = (long long)a->data[MATRIX_INDEX(*a, i, j)] + 
                           (long long)b->data[MATRIX_INDEX(*b, i, j)];
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[MATRIX_INDEX(*result, i, j)] = (int)sum;
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-10): ", 1, MAX_ROWS);
    int cols1 = read_int("Enter columns for matrix A (1-10): ", 1, MAX_COLS);
    matrix_init(&a, rows1, cols1);
    matrix_input(&a, "A");
    
    int rows2 = read_int("Enter rows for matrix B (1-10): ", 1, MAX_ROWS);
    int cols2 = read_int("Enter columns for matrix B (1-10): ", 1, MAX_COLS);
    matrix_init(&b, rows2, cols2);
    matrix_input(&b, "B");
    
    printf("\n");
    matrix_print(&a, "A");
    matrix