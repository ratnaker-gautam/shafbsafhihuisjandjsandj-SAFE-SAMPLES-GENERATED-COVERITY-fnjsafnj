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
        mat.data = calloc(rows * cols, sizeof(double));
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

double get_matrix_element(const struct Matrix *mat, size_t row, size_t col) {
    if (!matrix_is_valid(mat) || row >= mat->rows || col >= mat->cols) {
        return 0.0;
    }
    return mat->data[row * mat->cols + col];
}

int set_matrix_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (!matrix_is_valid(mat) || row >= mat->rows || col >= mat->cols) {
        return 0;
    }
    mat->data[row * mat->cols + col] = value;
    return 1;
}

struct MatrixResult matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    if (!matrix_is_valid(a) || !matrix_is_valid(b)) {
        strcpy(result.error_msg, "Invalid input matrices");
        return result;
    }
    if (a->cols != b->rows) {
        strcpy(result.error_msg, "Dimension mismatch for multiplication");
        return result;
    }
    struct Matrix product = create_matrix(a->rows, b->cols);
    if (!matrix_is_valid(&product)) {
        strcpy(result.error_msg, "Memory allocation failed");
        return result;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += get_matrix_element(a, i, k) * get_matrix_element(b, k, j);
            }
            set_matrix_element(&product, i, j, sum);
        }
    }
    result.success = 1;
    result.matrix = product;
    return result;
}

struct MatrixResult matrix_transpose(const struct Matrix *mat) {
    struct MatrixResult result = {0};
    if (!matrix_is_valid(mat)) {
        strcpy(result.error_msg, "Invalid input matrix");
        return result;
    }
    struct Matrix transposed = create_matrix(mat->cols, mat->rows);
    if (!matrix_is_valid(&transposed)) {
        strcpy(result.error_msg, "Memory allocation failed");
        return result;
    }
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            set_matrix_element(&transposed, j, i, get_matrix_element(mat, i, j));
        }
    }
    result.success = 1;
    result.matrix = transposed;
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (!matrix_is_valid(mat)) {
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

int read_double_safe(double *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    return endptr != buffer;
}

int main(void) {
    struct Matrix a = create_matrix(2, 2);
    struct Matrix b = create_matrix(2, 2);
    if (!matrix_is_valid(&a) || !matrix_is_valid(&b)) {
        printf("Failed to create matrices\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    printf("Enter 4 values for matrix A (2x2):\n");
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            double value;
            if (!read_double_safe(&value)) {
                printf("Invalid input\n");
                destroy_matrix(&a);
                destroy_matrix(&b);
                return 1;
            }
            set_matrix_element(&a, i, j, value);
        }
    }
    printf("Enter 4 values for matrix B (2x2):\n");
    for (size_t i = 0; i < 2; i++) {
        for