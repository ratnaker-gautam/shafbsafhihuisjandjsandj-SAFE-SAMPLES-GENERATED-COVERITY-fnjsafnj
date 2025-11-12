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
    if (mat == NULL || mat->data == NULL) {
        printf("Invalid matrix\n");
        return;
    }
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double value;
            if (get_matrix_element(mat, i, j, &value)) {
                printf("%8.2f ", value);
            }
        }
        printf("\n");
    }
}

int read_positive_size(const char *prompt, size_t *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    int scanned = scanf("%zu", value);
    if (scanned != 1 || *value == 0 || *value > 1000) return 0;
    return 1;
}

int read_double(const char *prompt, double *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    int scanned = scanf("%lf", value);
    return scanned == 1;
}

int main(void) {
    struct Matrix mat1, mat2, result;
    size_t rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    if (!read_positive_size("Enter rows for matrix 1: ", &rows1)) {
        printf("Invalid rows for matrix 1\n");
        return 1;
    }
    if (!read_positive_size("Enter columns for matrix 1: ", &cols1)) {
        printf("Invalid columns for matrix 1\n