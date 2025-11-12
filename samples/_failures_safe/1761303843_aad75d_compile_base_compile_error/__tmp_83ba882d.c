//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUB 2
#define MATRIX_OP_MUL 3

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_mul(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;

    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }

    if (value < min || value > max) {
        return -1;
    }

    return (int)value;
}

void read_matrix(Matrix* mat, const char* name) {
    int i, j;
    char prompt[32];

    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    if (mat->rows == -1) {
        printf("Invalid rows input\n");
        exit(1);
    }

    mat->cols = read_int("Columns: ", MIN_DIM, MAX_COLS);
    if (mat->cols == -1) {
        printf("Invalid columns input\n");
        exit(1);
    }

    printf("Enter elements for matrix %s:\n", name);
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            snprintf(prompt, sizeof(prompt), "[%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
            if (mat->data[i][j] == -1) {
                printf("Invalid element input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    int i, j;
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    int i, j;
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }

    result->rows = a->rows;
    result->cols = a->cols;

    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            int sum = safe_add(a->data[i][j], b->data[i][j]);
            if (sum == 0 && (a->data[i][j] != 0 || b->data[i][j] != 0)) {
                return 0;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_sub(const Matrix* a, const Matrix* b, Matrix* result) {
    int i, j;
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }

    result->rows = a->rows;
    result->cols = a->cols;

    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            int diff = safe_add(a->data[i][j], -b->data[i][j]);
            if (diff == 0 && (a->data[i][j] != b->data[i][j])) {
                return 0;
            }
            result->data[i][j] = diff;
        }
    }
    return 1;
}

int matrix_mul(const Matrix* a, const Matrix* b, Matrix* result) {
    int i, j, k;
    if (a->cols != b->rows) {
        return 0;
    }

    result->rows = a->rows;
    result->cols = b->cols;

    for (i = 0; i < a->rows