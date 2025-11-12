//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
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

int get_matrix_element(const struct Matrix *mat, size_t row, size_t col, double *result) {
    if (mat == NULL || mat->data == NULL || result == NULL) return 0;
    if (row >= mat->rows || col >= mat->cols) return 0;
    *result = mat->data[row * mat->cols + col];
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
            double val1, val2;
            if (get_matrix_element(a, i, j, &val1) && get_matrix_element(b, i, j, &val2)) {
                set_matrix_element(&result, i, j, val1 + val2);
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
                double val1, val2;
                if (get_matrix_element(a, i, k, &val1) && get_matrix_element(b, k, j, &val2)) {
                    sum += val1 * val2;
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
            double val;
            if (get_matrix_element(mat, i, j, &val)) {
                printf("%8.2f", val);
            }
        }
        printf("\n");
    }
}

int read_matrix_from_input(struct Matrix *mat) {
    if (mat == NULL) return 0;
    printf("Enter number of rows: ");
    int rows;
    if (scanf("%d", &rows) != 1 || rows <= 0 || rows > 100) return 0;
    printf("Enter number of columns: ");
    int cols;
    if (scanf("%d", &cols) != 1 || cols <= 0 || cols > 100) return 0;
    *mat = create_matrix(rows, cols);
    if (mat->data == NULL) return 0;
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double val;
            if (scanf("%lf", &val) != 1) {
                destroy_matrix(mat);
                return 0;
            }
            set_matrix_element(mat, i, j, val);
        }
    }
    return 1;
}

int main() {
    struct Matrix mat1, mat2;
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix_from_input(&mat