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
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            product.data[i * product.cols + j] = sum;
        }
    }
    
    result.success = 1;
    result.matrix = product;
    return result;
}

struct MatrixResult add_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0, {0, 0, NULL}, ""};
    
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        strcpy(result.error_msg, "Invalid input matrices");
        return result;
    }
    
    if (a->rows != b->rows || a->cols != b->cols) {
        strcpy(result.error_msg, "Dimension mismatch for addition");
        return result;
    }
    
    struct Matrix sum = create_matrix(a->rows, a->cols);
    if (sum.data == NULL) {
        strcpy(result.error_msg, "Memory allocation failed");
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
    return (endptr != buffer);
}

int read_size_t(const char *prompt, size_t *value) {
    char buffer[64];
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    char *endptr;
    unsigned long long temp = strtoull(buffer, &endptr, 10);
    if (endptr == buffer || temp > SIZE_MAX) return 0;
    
    *value = (size_t)temp;
    return 1;
}

int main(void) {
    struct Matrix mat1 = {0, 0, NULL};
    struct Matrix mat2 = {0, 0, NULL};
    
    printf("Matrix Operations Program\n");
    printf("Creating first matrix:\n");
    
    size_t rows1, cols1;
    if (!read_size_t("Enter rows: ", &rows1) || !read_size_t("Enter columns: ", &cols1)) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    mat1 = create_matrix(rows1, cols1);
    if (mat1.data == NULL) {
        printf("Failed to create first matrix\n");
        return 1;
    }
    
    printf("Enter %zu elements for first matrix:\n", rows1 * cols1);