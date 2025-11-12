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

struct MatrixResult {
    int success;
    struct Matrix matrix;
    char error_msg[256];
};

struct Matrix create_matrix(size_t rows, size_t cols) {
    struct Matrix mat = {0, 0, NULL};
    if (rows == 0 || cols == 0) return mat;
    if (rows > SIZE_MAX / cols / sizeof(double)) return mat;
    
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) return mat;
    
    mat.rows = rows;
    mat.cols = cols;
    memset(mat.data, 0, rows * cols * sizeof(double));
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat == NULL) return;
    free(mat->data);
    mat->data = NULL;
    mat->rows = 0;
    mat->cols = 0;
}

struct MatrixResult matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    result.success = 0;
    
    if (a == NULL || b == NULL) {
        strncpy(result.error_msg, "Null matrix input", sizeof(result.error_msg));
        return result;
    }
    
    if (a->cols != b->rows) {
        strncpy(result.error_msg, "Dimension mismatch", sizeof(result.error_msg));
        return result;
    }
    
    if (a->rows == 0 || a->cols == 0 || b->rows == 0 || b->cols == 0) {
        strncpy(result.error_msg, "Empty matrix", sizeof(result.error_msg));
        return result;
    }
    
    struct Matrix product = create_matrix(a->rows, b->cols);
    if (product.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg));
        return result;
    }
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            product.data[i * product.cols + j] = sum;
        }
    }
    
    result.success = 1;
    result.matrix = product;
    return result;
}

struct MatrixResult matrix_transpose(const struct Matrix *mat) {
    struct MatrixResult result = {0};
    result.success = 0;
    
    if (mat == NULL) {
        strncpy(result.error_msg, "Null matrix input", sizeof(result.error_msg));
        return result;
    }
    
    if (mat->rows == 0 || mat->cols == 0) {
        strncpy(result.error_msg, "Empty matrix", sizeof(result.error_msg));
        return result;
    }
    
    struct Matrix transposed = create_matrix(mat->cols, mat->rows);
    if (transposed.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg));
        return result;
    }
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            transposed.data[j * transposed.cols + i] = mat->data[i * mat->cols + j];
        }
    }
    
    result.success = 1;
    result.matrix = transposed;
    return result;
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

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f ", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

void fill_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j