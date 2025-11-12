//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
};

struct Matrix create_matrix(size_t rows, size_t cols) {
    struct Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols / sizeof(double)) {
        mat.data = NULL;
        return mat;
    }
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
        return mat;
    }
    memset(mat.data, 0, rows * cols * sizeof(double));
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat != NULL) {
        free(mat->data);
        mat->data = NULL;
        mat->rows = 0;
        mat->cols = 0;
    }
}

int set_matrix_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (mat == NULL || mat->data == NULL) return 0;
    if (row >= mat->rows || col >= mat->cols) return 0;
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_matrix_element(const struct Matrix *mat, size_t row, size_t col) {
    if (mat == NULL || mat->data == NULL) return 0.0;
    if (row >= mat->rows || col >= mat->cols) return 0.0;
    return mat->data[row * mat->cols + col];
}

struct Matrix matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        return result;
    }
    if (a->cols != b->rows) {
        return result;
    }
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) {
        return result;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += get_matrix_element(a, i, k) * get_matrix_element(b, k, j);
            }
            set_matrix_element(&result, i, j, sum);
        }
    }
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        printf("Invalid matrix\n");
        return;
    }
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_matrix_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_positive_size(const char *prompt) {
    char buffer[32];
    long value;
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    char *endptr;
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value <= 0 || value > 100) {
        return -1;
    }
    return (int)value;
}

double read_double_value(const char *prompt) {
    char buffer[32];
    double value;
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0.0;
    }
    char *endptr;
    value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0.0;
    }
    return value;
}

int main(void) {
    struct Matrix mat1, mat2, result;
    int rows1, cols1, rows2, cols2;

    printf("Matrix Multiplication Program\n");
    printf("Matrix A dimensions:\n");
    rows1 = read_positive_size("Rows (1-100): ");
    cols1 = read_positive_size("Columns (1-100): ");
    if (rows1 <= 0 || cols1 <= 0) {
        printf("Invalid dimensions for matrix A\n");
        return 1;
    }

    printf("Matrix B dimensions:\n");
    rows2 = read_positive_size("Rows (1-100): ");
    cols2 = read_positive_size("Columns (1-100): ");
    if (rows2 <= 0 || cols2 <= 0) {
        printf("Invalid dimensions for matrix B\n");
        return 1;
    }

    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        printf("Columns of A (%d) must equal rows of B (%d)\n", cols1, rows2);
        return 1;
    }

    mat1 = create_matrix(rows1, cols1);
    mat2 = create_matrix(rows2, cols2);
    if (mat1.data == NULL || mat2.data == NULL