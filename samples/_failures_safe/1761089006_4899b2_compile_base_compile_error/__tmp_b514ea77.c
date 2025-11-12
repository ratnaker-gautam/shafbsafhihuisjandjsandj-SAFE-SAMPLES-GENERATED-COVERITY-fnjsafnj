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
    char error_msg[64];
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

int matrix_is_valid(const struct Matrix *mat) {
    return mat != NULL && mat->rows > 0 && mat->cols > 0 && mat->data != NULL;
}

struct MatrixResult matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    if (!matrix_is_valid(a) || !matrix_is_valid(b)) {
        strncpy(result.error_msg, "Invalid input matrices", sizeof(result.error_msg)-1);
        return result;
    }
    if (a->cols != b->rows) {
        strncpy(result.error_msg, "Dimension mismatch for multiplication", sizeof(result.error_msg)-1);
        return result;
    }
    struct Matrix product = create_matrix(a->rows, b->cols);
    if (product.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg)-1);
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

struct MatrixResult matrix_add(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    if (!matrix_is_valid(a) || !matrix_is_valid(b)) {
        strncpy(result.error_msg, "Invalid input matrices", sizeof(result.error_msg)-1);
        return result;
    }
    if (a->rows != b->rows || a->cols != b->cols) {
        strncpy(result.error_msg, "Dimension mismatch for addition", sizeof(result.error_msg)-1);
        return result;
    }
    struct Matrix sum = create_matrix(a->rows, a->cols);
    if (sum.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg)-1);
        return result;
    }
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        sum.data[i] = a->data[i] + b->data[i];
    }
    result.success = 1;
    result.matrix = sum;
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (!matrix_is_valid(mat)) {
        printf("Invalid matrix\n");
        return;
    }
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int read_double(double *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    return endptr != buffer;
}

int read_size_t(size_t *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    long long val = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || val < 0 || val > SIZE_MAX) {
        return 0;
    }
    *value = (size_t)val;
    return 1;
}

struct Matrix read_matrix() {
    struct Matrix mat = {0};
    printf("Enter number of rows: ");
    if (!read_size_t(&mat.rows) || mat.rows == 0) {
        return mat;
    }
    printf("Enter number of columns: ");
    if (!read_size_t(&mat.cols) || mat.cols == 0) {
        return mat;
    }
    mat = create_matrix(mat.rows, mat.cols);
    if (mat.data == NULL) {
        return mat;
    }
    printf("Enter matrix elements row by row:\n");
    for (size_t i = 0; i < mat.rows; i++) {
        for (size_t j = 0; j < mat