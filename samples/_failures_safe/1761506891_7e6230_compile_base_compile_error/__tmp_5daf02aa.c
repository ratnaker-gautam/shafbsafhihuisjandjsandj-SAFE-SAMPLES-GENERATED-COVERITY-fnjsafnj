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

struct Matrix create_matrix(size_t rows, size_t cols) {
    struct Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = NULL;
    
    if (rows == 0 || cols == 0) {
        return mat;
    }
    
    if (SIZE_MAX / rows < cols) {
        return mat;
    }
    
    size_t total_elements = rows * cols;
    if (total_elements > SIZE_MAX / sizeof(double)) {
        return mat;
    }
    
    mat.data = malloc(total_elements * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
        return mat;
    }
    
    for (size_t i = 0; i < total_elements; i++) {
        mat.data[i] = 0.0;
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

int set_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (mat == NULL || mat->data == NULL) {
        return 0;
    }
    if (row >= mat->rows || col >= mat->cols) {
        return 0;
    }
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_element(const struct Matrix *mat, size_t row, size_t col) {
    if (mat == NULL || mat->data == NULL) {
        return 0.0;
    }
    if (row >= mat->rows || col >= mat->cols) {
        return 0.0;
    }
    return mat->data[row * mat->cols + col];
}

struct Matrix multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        return result;
    }
    
    if (a->cols != b->rows) {
        return result;
    }
    
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) {
        return result;
    }
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += get_element(a, i, k) * get_element(b, k, j);
            }
            set_element(&result, i, j, sum);
        }
    }
    
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        printf("Invalid matrix\n");
        return;
    }
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_safe_size_t(size_t *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char *endptr;
    long long temp = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    
    if (temp < 0 || temp > SIZE_MAX) {
        return 0;
    }
    
    *value = (size_t)temp;
    return 1;
}

int read_safe_double(double *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    
    return 1;
}

int main(void) {
    struct Matrix a, b, result;
    size_t rows, cols;
    double value;
    
    printf("Enter rows for matrix A: ");
    if (!read_safe_size_t(&rows) || rows == 0 || rows > 100) {
        printf("Invalid rows\n");
        return 1;
    }
    
    printf("Enter columns for matrix A: ");
    if (!read_safe_size_t(&cols) || cols == 0 || cols > 100) {
        printf("Invalid columns\n");
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