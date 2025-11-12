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

struct MatrixResult matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
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
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result.matrix.data[i * b->cols + j] = sum;
        }
    }
    
    result.success = 1;
    return result;
}

struct MatrixResult matrix_transpose(const struct Matrix *mat) {
    struct MatrixResult result = {0};
    
    if (mat == NULL || mat->data == NULL) {
        strncpy(result.error_msg, "Invalid input matrix", sizeof(result.error_msg)-1);
        return result;
    }
    
    result.matrix = create_matrix(mat->cols, mat->rows);
    if (result.matrix.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg)-1);
        return result;
    }
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            result.matrix.data[j * mat->rows + i] = mat->data[i * mat->cols + j];
        }
    }
    
    result.success = 1;
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

void read_matrix_values(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double val;
            printf("Enter value for [%zu][%zu]: ", i, j);
            if (!read_double("", &val)) {
                val = 0.0;
            }
            mat->data[i * mat->cols + j] = val;
        }
    }
}

int main(void) {
    struct Matrix a = {0}, b = {0};
    int choice;
    char buffer[16];
    
    printf("Matrix Operations Demo\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf