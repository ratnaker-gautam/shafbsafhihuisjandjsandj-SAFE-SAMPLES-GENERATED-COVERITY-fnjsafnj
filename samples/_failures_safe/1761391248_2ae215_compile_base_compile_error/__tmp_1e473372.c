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
    struct Matrix mat = {0, 0, NULL};
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols / sizeof(double)) {
        return mat;
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data = calloc(rows * cols, sizeof(double));
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat != NULL && mat->data != NULL) {
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

struct MatrixResult multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0, {0, 0, NULL}, ""};
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        strcpy(result.error_msg, "Null matrix input");
        return result;
    }
    if (a->cols != b->rows) {
        strcpy(result.error_msg, "Dimension mismatch");
        return result;
    }
    if (a->rows > SIZE_MAX / b->cols / sizeof(double)) {
        strcpy(result.error_msg, "Result matrix too large");
        return result;
    }
    struct Matrix product = create_matrix(a->rows, b->cols);
    if (product.data == NULL) {
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

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_matrix_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_safe_double(double *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    return (endptr != buffer);
}

int main(void) {
    struct Matrix mat1 = {0, 0, NULL};
    struct Matrix mat2 = {0, 0, NULL};
    size_t rows1, cols1, rows2, cols2;
    printf("Enter dimensions for matrix 1 (rows cols): ");
    if (scanf("%zu %zu", &rows1, &cols1) != 2 || rows1 == 0 || cols1 == 0) {
        printf("Invalid dimensions for matrix 1\n");
        return 1;
    }
    while (getchar() != '\n');
    mat1 = create_matrix(rows1, cols1);
    if (mat1.data == NULL) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    printf("Enter %zu elements for matrix 1 (row-major):\n", rows1 * cols1);
    for (size_t i = 0; i < rows1; i++) {
        for (size_t j = 0; j < cols1; j++) {
            double value;
            if (!read_safe_double(&value)) {
                printf("Invalid input for matrix element\n");
                destroy_matrix(&mat1);
                return 1;
            }
            set_matrix_element(&mat1, i, j, value);
        }
    }
    printf("Enter dimensions for matrix 2 (rows cols): ");
    if (scanf("%zu %zu", &rows2, &cols2) != 2 || rows2 == 0 || cols2 == 0) {
        printf("Invalid dimensions for matrix 2\n");
        destroy_matrix(&mat1);
        return 1;
    }
    while (getchar() != '\n');
    mat2 = create