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
    if (mat.data != NULL) {
        memset(mat.data, 0, rows * cols * sizeof(double));
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
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_element(const struct Matrix *mat, size_t row, size_t col) {
    if (mat == NULL || mat->data == NULL) return 0.0;
    if (row >= mat->rows || col >= mat->cols) return 0.0;
    return mat->data[row * mat->cols + col];
}

struct Matrix add_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
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
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
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
    if (mat == NULL || mat->data == NULL) {
        printf("Invalid matrix\n");
        return;
    }
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_double(const char *prompt, double *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) return 0;
    return 1;
}

int read_size_t(const char *prompt, size_t *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long long temp = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || temp < 0 || temp > SIZE_MAX) return 0;
    *value = (size_t)temp;
    return 1;
}

int main(void) {
    struct Matrix mat1, mat2, result;
    size_t rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    if (!read_size_t("Enter rows for matrix 1: ", &rows1) || rows1 == 0) {
        printf("Invalid rows for matrix 1\n");
        return 1;
    }
    if (!read_size_t("Enter columns for matrix 1: ", &cols1) || cols1 == 0) {
        printf("Invalid columns for matrix 1\n");
        return 1;