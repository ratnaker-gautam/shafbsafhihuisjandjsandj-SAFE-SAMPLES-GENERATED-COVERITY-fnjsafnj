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
    struct MatrixResult result = {0};
    
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        strncpy(result.error_msg, "Invalid input matrices", sizeof(result.error_msg)-1);
        return result;
    }
    
    if (a->cols != b->rows) {
        strncpy(result.error_msg, "Dimension mismatch for multiplication", sizeof(result.error_msg)-1);
        return result;
    }
    
    result.matrix = create_matrix(a->rows, b->cols);
    if (result.matrix.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg)-1);
        return result;
    }
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                if (k < a->cols && i < a->rows && k < b->rows && j < b->cols) {
                    sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
                }
            }
            result.matrix.data[i * b->cols + j] = sum;
        }
    }
    
    result.success = 1;
    return result;
}

struct MatrixResult add_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        strncpy(result.error_msg, "Invalid input matrices", sizeof(result.error_msg)-1);
        return result;
    }
    
    if (a->rows != b->rows || a->cols != b->cols) {
        strncpy(result.error_msg, "Dimension mismatch for addition", sizeof(result.error_msg)-1);
        return result;
    }
    
    result.matrix = create_matrix(a->rows, a->cols);
    if (result.matrix.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg)-1);
        return result;
    }
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            result.matrix.data[i * a->cols + j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
        }
    }
    
    result.success = 1;
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

int read_double(double *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    return sscanf(buffer, "%lf", value) == 1;
}

int main() {
    struct Matrix a = {0}, b = {0};
    size_t rows, cols;
    
    printf("Enter dimensions for matrix A (rows cols): ");
    if (scanf("%zu %zu", &rows, &cols) != 2 || rows == 0 || cols == 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    a = create_matrix(rows, cols);
    if (a.data == NULL) {
        printf("Failed to create matrix A\n");
        return 1;
    }
    
    printf("Enter %zu elements for matrix A:\n", rows * cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double value;
            if (!read_double(&value)) {
                printf("Invalid input\n");
                destroy_matrix(&a);
                return 1;