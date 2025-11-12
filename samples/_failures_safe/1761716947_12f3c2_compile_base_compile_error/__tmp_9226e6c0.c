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

int get_matrix_element(const struct Matrix *mat, size_t row, size_t col, double *value) {
    if (mat == NULL || mat->data == NULL || value == NULL) return 0;
    if (row >= mat->rows || col >= mat->cols) return 0;
    *value = mat->data[row * mat->cols + col];
    return 1;
}

struct Matrix add_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
    if (a->rows != b->rows || a->cols != b->cols) return result;
    result = create_matrix(a->rows, a->cols);
    if (result.data == NULL) return result;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            double val_a, val_b;
            if (get_matrix_element(a, i, j, &val_a) && get_matrix_element(b, i, j, &val_b)) {
                set_matrix_element(&result, i, j, val_a + val_b);
            }
        }
    }
    return result;
}

struct Matrix multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
    if (a->cols != b->rows) return result;
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) return result;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                double val_a, val_b;
                if (get_matrix_element(a, i, k, &val_a) && get_matrix_element(b, k, j, &val_b)) {
                    sum += val_a * val_b;
                }
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
            double value;
            if (get_matrix_element(mat, i, j, &value)) {
                printf("%8.2f", value);
            }
        }
        printf("\n");
    }
}

int read_matrix_from_input(struct Matrix *mat) {
    if (mat == NULL) return 0;
    printf("Enter number of rows: ");
    size_t rows;
    if (scanf("%zu", &rows) != 1 || rows == 0 || rows > 100) return 0;
    printf("Enter number of columns: ");
    size_t cols;
    if (scanf("%zu", &cols) != 1 || cols == 0 || cols > 100) return 0;
    *mat = create_matrix(rows, cols);
    if (mat->data == NULL) return 0;
    printf("Enter matrix elements row by row:\n");
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double value;
            if (scanf("%lf", &value) != 1) {
                destroy_matrix(mat);
                return 0;
            }
            set_matrix_element(mat, i, j, value);
        }
    }
    return 1;
}

int main(void) {
    struct Matrix mat_a, mat_b;
    printf("Matrix A:\n");
    if (!read_matrix_from_input(&mat_a)) {