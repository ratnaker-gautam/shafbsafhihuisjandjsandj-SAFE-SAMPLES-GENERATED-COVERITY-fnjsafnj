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
    if (mat != NULL && mat->data != NULL) {
        free(mat->data);
        mat->data = NULL;
        mat->rows = 0;
        mat->cols = 0;
    }
}

struct MatrixResult multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0, {0, 0, NULL}, ""};
    
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
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

int read_double_safe(double *value) {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    char *endptr;
    *value = strtod(buffer, &endptr);
    return (endptr != buffer && *endptr == '\n');
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

int main(void) {
    struct Matrix mat1 = {0, 0, NULL};
    struct Matrix mat2 = {0, 0, NULL};
    
    printf("Enter dimensions for first matrix (rows cols): ");
    size_t rows1, cols1;
    if (scanf("%zu %zu", &rows1, &cols1) != 2 || rows1 == 0 || cols1 == 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    mat1 = create_matrix(rows1, cols1);
    if (mat1.data == NULL) {
        printf("Failed to create first matrix\n");
        return 1;
    }
    
    printf("Enter %zu elements for first matrix:\n", rows1 * cols1);
    for (size_t i = 0; i < rows1 * cols1; i++) {
        if (!read_double_safe(&mat1.data[i])) {
            printf("Invalid input\n");
            destroy_matrix(&mat1);
            return 1;
        }
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    size_t rows2, cols2;
    if (scanf("%zu %zu", &rows2, &cols2) != 2 || rows2 == 0 || cols2 == 0) {
        printf("Invalid dimensions\n");
        destroy_matrix(&mat1);
        return 1;
    }
    
    mat2 = create_matrix(rows2, cols2);
    if (mat2.data == NULL) {
        printf("Failed to create second matrix\n");
        destroy_matrix(&mat1);
        return 1;
    }
    
    printf("Enter %zu elements for second matrix:\n", rows2 * cols2);
    for (size_t i = 0; i < rows2 * cols2; i++) {
        if (!read_double_safe(&mat2.data[i])) {
            printf("Invalid input\n");
            destroy_matrix(&mat1);
            destroy_matrix(&mat2);
            return 1;
        }
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    struct MatrixResult result = multiply_matrices(&mat1, &