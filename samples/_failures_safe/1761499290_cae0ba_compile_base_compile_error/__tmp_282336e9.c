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
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

#define MATRIX_INIT { .data = {{0}}, .rows = 0, .cols = 0 }

#define VALID_DIMENSION(dim) ((dim) > 0 && (dim) <= MAX_ROWS)
#define VALID_INDEX(row, col, mat) ((row) >= 0 && (row) < (mat).rows && (col) >= 0 && (col) < (mat).cols)

void matrix_init(Matrix *mat, int rows, int cols) {
    if (!mat || !VALID_DIMENSION(rows) || !VALID_DIMENSION(cols)) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void matrix_print(const Matrix *mat) {
    if (!mat || mat->rows == 0 || mat->cols == 0) {
        printf("Invalid matrix\n");
        return;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%4d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (!VALID_DIMENSION(a->rows) || !VALID_DIMENSION(a->cols)) return 0;
    
    matrix_init(result, a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->cols != b->rows) return 0;
    if (!VALID_DIMENSION(a->rows) || !VALID_DIMENSION(a->cols) || !VALID_DIMENSION(b->cols)) return 0;
    
    matrix_init(result, a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    return 0;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    long value;
    char *endptr;
    
    printf("%s", prompt);
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return -1;
    }
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || value < min || value > max) {
        return -1;
    }
    return (int)value;
}

void matrix_read(Matrix *mat) {
    int rows, cols;
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_ROWS);
    if (rows == -1) {
        printf("Invalid rows\n");
        return;
    }
    
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_COLS);
    if (cols == -1) {
        printf("Invalid columns\n");
        return;
    }
    
    matrix_init(mat, rows, cols);
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid input, using 0\n");
                val = 0;
            }
            mat->data[i][j] = val;