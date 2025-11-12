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

int matrix_index_valid(const struct Matrix *mat, size_t row, size_t col) {
    return (mat != NULL && mat->data != NULL && 
            row < mat->rows && col < mat->cols);
}

double get_matrix_element(const struct Matrix *mat, size_t row, size_t col) {
    if (!matrix_index_valid(mat, row, col)) return 0.0;
    return mat->data[row * mat->cols + col];
}

int set_matrix_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (!matrix_index_valid(mat, row, col)) return 0;
    mat->data[row * mat->cols + col] = value;
    return 1;
}

struct MatrixResult matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    result.success = 0;
    strcpy(result.error_msg, "Unknown error");
    
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        strcpy(result.error_msg, "Invalid input matrices");
        return result;
    }
    
    if (a->cols != b->rows) {
        strcpy(result.error_msg, "Dimension mismatch for multiplication");
        return result;
    }
    
    if (a->rows == 0 || a->cols == 0 || b->rows == 0 || b->cols == 0) {
        strcpy(result.error_msg, "Empty matrix");
        return result;
    }
    
    if (a->rows > SIZE_MAX / b->cols / sizeof(double)) {
        strcpy(result.error_msg, "Result matrix too large");
        return result;
    }
    
    result.matrix = create_matrix(a->rows, b->cols);
    if (result.matrix.data == NULL) {
        strcpy(result.error_msg, "Memory allocation failed");
        return result;
    }
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += get_matrix_element(a, i, k) * get_matrix_element(b, k, j);
            }
            set_matrix_element(&result.matrix, i, j, sum);
        }
    }
    
    result.success = 1;
    strcpy(result.error_msg, "Success");
    return result;
}

struct MatrixResult matrix_transpose(const struct Matrix *mat) {
    struct MatrixResult result = {0};
    result.success = 0;
    strcpy(result.error_msg, "Unknown error");
    
    if (mat == NULL || mat->data == NULL) {
        strcpy(result.error_msg, "Invalid input matrix");
        return result;
    }
    
    if (mat->rows == 0 || mat->cols == 0) {
        strcpy(result.error_msg, "Empty matrix");
        return result;
    }
    
    if (mat->rows > SIZE_MAX / mat->cols / sizeof(double)) {
        strcpy(result.error_msg, "Result matrix too large");
        return result;
    }
    
    result.matrix = create_matrix(mat->cols, mat->rows);
    if (result.matrix.data == NULL) {
        strcpy(result.error_msg, "Memory allocation failed");
        return result;
    }
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            set_matrix_element(&result.matrix, j, i, get_matrix_element(mat, i, j));
        }
    }
    
    result.success = 1;
    strcpy(result.error_msg, "Success");
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        printf("Invalid matrix\n");
        return;
    }
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f ", get_matrix_element(mat, i, j));
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

int read