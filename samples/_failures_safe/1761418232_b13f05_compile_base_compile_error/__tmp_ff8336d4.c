//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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
    for (size_t i = 0; i < rows * cols; i++) {
        mat.data[i] = 0.0;
    }
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

struct MatrixResult matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0, {0, 0, NULL}, ""};
    
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        strcpy(result.error_msg, "Invalid input matrices");
        return result;
    }
    
    if (a->cols != b->rows) {
        strcpy(result.error_msg, "Dimension mismatch for multiplication");
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
                if (a->data[i * a->cols + k] > 0.0 && b->data[k * b->cols + j] > 0.0) {
                    if (a->data[i * a->cols + k] > DBL_MAX / b->data[k * b->cols + j]) {
                        destroy_matrix(&product);
                        strcpy(result.error_msg, "Multiplication overflow");
                        return result;
                    }
                }
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            product.data[i * b->cols + j] = sum;
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
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int read_double(const char *prompt, double *value) {
    char buffer[64];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') return 0;
    return 1;
}

int read_size_t(const char *prompt, size_t *value) {
    char buffer[64];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    char *endptr;
    long long temp = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (temp < 0 || temp > SIZE_MAX) return 0;
    
    *value = (size_t)temp;
    return 1;
}

int main(void) {
    struct Matrix mat1 = {0, 0, NULL};
    struct Matrix mat2 = {0, 0, NULL};
    
    size_t rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    
    if (!read_size_t("Enter rows for matrix 1: ", &rows1) || rows1 == 0) {
        printf("Invalid rows for matrix 1\n");
        return 1;
    }
    
    if (!read_size_t("Enter columns for matrix 1: ", &cols1) || cols1 == 0) {
        printf("Invalid columns for matrix 1\n");
        return 1;
    }
    
    mat1 = create_matrix(rows1, cols1);
    if (mat1.data == NULL) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    
    printf("Enter elements for matrix 1 (%zux%zu):\n", rows1, cols1);
    for (size_t i = 0; i < rows1; i++) {
        for (size_t j = 0; j <