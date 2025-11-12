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
    if (rows == 0 || cols == 0) return mat;
    if (SIZE_MAX / cols < rows) return mat;
    size_t total = rows * cols;
    if (total > SIZE_MAX / sizeof(double)) return mat;
    mat.data = malloc(total * sizeof(double));
    if (mat.data == NULL) return mat;
    for (size_t i = 0; i < total; i++) {
        mat.data[i] = 0.0;
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

int set_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (mat == NULL || mat->data == NULL) return 0;
    if (row >= mat->rows || col >= mat->cols) return 0;
    size_t index = row * mat->cols + col;
    mat->data[index] = value;
    return 1;
}

double get_element(const struct Matrix *mat, size_t row, size_t col) {
    if (mat == NULL || mat->data == NULL) return 0.0;
    if (row >= mat->rows || col >= mat->cols) return 0.0;
    size_t index = row * mat->cols + col;
    return mat->data[index];
}

struct Matrix add_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL) return result;
    if (a->rows != b->rows || a->cols != b->cols) return result;
    result = create_matrix(a->rows, a->cols);
    if (result.data == NULL) return result;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            double sum = get_element(a, i, j) + get_element(b, i, j);
            set_element(&result, i, j, sum);
        }
    }
    return result;
}

struct Matrix multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL) return result;
    if (a->cols != b->rows) return result;
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) return result;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += get_element(a, i, k) * get_element(b, k, j);
            }
            set_element(&result, i, j, sum);
        }
    }
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_matrix(struct Matrix *mat) {
    if (mat == NULL) return 0;
    printf("Enter number of rows: ");
    if (scanf("%zu", &mat->rows) != 1) return 0;
    printf("Enter number of columns: ");
    if (scanf("%zu", &mat->cols) != 1) return 0;
    if (mat->rows == 0 || mat->cols == 0) return 0;
    if (SIZE_MAX / mat->cols < mat->rows) return 0;
    size_t total = mat->rows * mat->cols;
    if (total > SIZE_MAX / sizeof(double)) return 0;
    mat->data = malloc(total * sizeof(double));
    if (mat->data == NULL) return 0;
    printf("Enter %zu elements row-wise:\n", total);
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double value;
            if (scanf("%lf", &value) != 1) {
                free(mat->data);
                mat->data = NULL;
                return 0;
            }
            set_element(mat, i, j, value);
        }
    }
    return 1;
}

int main(void) {
    struct Matrix mat1 = create_matrix(0, 0);
    struct Matrix mat2 = create_matrix(0, 0);
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&mat1)) {
        printf