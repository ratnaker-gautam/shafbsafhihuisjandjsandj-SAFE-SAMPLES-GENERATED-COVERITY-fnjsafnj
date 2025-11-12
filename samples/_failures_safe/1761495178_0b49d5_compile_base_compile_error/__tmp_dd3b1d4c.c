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

struct MatrixResult matrix_add(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    result.success = 0;
    
    if (a == NULL || b == NULL) {
        strncpy(result.error_msg, "Null matrix input", sizeof(result.error_msg));
        return result;
    }
    
    if (a->rows != b->rows || a->cols != b->cols) {
        strncpy(result.error_msg, "Dimension mismatch", sizeof(result.error_msg));
        return result;
    }
    
    if (a->rows == 0 || a->cols == 0) {
        strncpy(result.error_msg, "Empty matrix", sizeof(result.error_msg));
        return result;
    }
    
    struct Matrix sum = create_matrix(a->rows, a->cols);
    if (sum.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg));
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
    if (mat == NULL || mat->data == NULL) {
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

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) return 0;
    return 1;
}

int main(void) {
    struct Matrix a, b;
    size_t rows, cols;
    
    printf("Enter dimensions for matrix A (rows cols): ");
    if (scanf("%zu %zu", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    a = create_matrix(rows, cols);
    if (a.data == NULL) {
        printf("Failed to create matrix A\n");
        return 1;
    }
    
    printf("Enter %zu elements for matrix A:\n", rows * cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double value;