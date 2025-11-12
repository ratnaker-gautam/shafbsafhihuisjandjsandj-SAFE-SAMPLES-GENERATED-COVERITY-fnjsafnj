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
    mat.data = NULL;
    if (rows > 0 && cols > 0 && rows <= SIZE_MAX / cols / sizeof(double)) {
        mat.data = malloc(rows * cols * sizeof(double));
        if (mat.data != NULL) {
            memset(mat.data, 0, rows * cols * sizeof(double));
        }
    }
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
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
    if (a->cols != b->rows) return result;
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) return result;
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
    if (mat == NULL || mat->data == NULL) return;
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_matrix_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_safe_size_t(size_t *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long long val = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (val < 0 || val > SIZE_MAX) return 0;
    *value = (size_t)val;
    return 1;
}

double read_safe_double() {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0.0;
    char *endptr;
    double val = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') return 0.0;
    return val;
}

struct Matrix read_matrix() {
    struct Matrix mat = create_matrix(0, 0);
    size_t rows, cols;
    printf("Enter number of rows: ");
    if (!read_safe_size_t(&rows) || rows == 0) return mat;
    printf("Enter number of columns: ");
    if (!read_safe_size_t(&cols) || cols == 0) return mat;
    mat = create_matrix(rows, cols);
    if (mat.data == NULL) return mat;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("Enter element [%zu][%zu]: ", i, j);
            double val = read_safe_double();
            set_matrix_element(&mat, i, j, val);
        }
    }
    return mat;
}

int main() {
    struct Matrix a = read_matrix();
    if (a.data == NULL) {
        printf("Failed to create matrix A\n");
        return 1;
    }
    struct Matrix b = read_matrix();
    if (b.data == NULL) {
        printf("Failed to create matrix B\n");
        destroy_matrix(&a);
        return 1;
    }
    struct Matrix c = matrix_multiply(&a, &b);
    if (c.data == NULL) {
        printf("Matrix multiplication failed\n");
    } else {
        printf("Result matrix:\n");
        print_matrix(&c);
        destroy_matrix(&c);
    }
    destroy_matrix(&a);
    destroy_matrix(&b);
    return 0;
}