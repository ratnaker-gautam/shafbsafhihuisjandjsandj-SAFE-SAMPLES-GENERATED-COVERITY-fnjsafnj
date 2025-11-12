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
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
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

struct Matrix matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
    if (a->cols != b->rows) return result;
    
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += get_matrix_element(a, i, k) * get_matrix_element(b, k, j);
            }
            set_matrix_element(&result, i, j, sum);
        }
    }
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
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    return (endptr != buffer);
}

int read_safe_size_t(size_t *value) {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long long val = strtoll(buffer, &endptr, 10);
    if (endptr == buffer) return 0;
    if (val < 0 || val > SIZE_MAX) return 0;
    *value = (size_t)val;
    return 1;
}

struct Matrix read_matrix_input(void) {
    struct Matrix mat = create_matrix(0, 0);
    size_t rows, cols;
    
    printf("Enter number of rows: ");
    if (!read_safe_size_t(&rows) || rows == 0 || rows > 100) {
        printf("Invalid row count\n");
        return mat;
    }
    
    printf("Enter number of columns: ");
    if (!read_safe_size_t(&cols) || cols == 0 || cols > 100) {
        printf("Invalid column count\n");
        return mat;
    }
    
    mat = create_matrix(rows, cols);
    if (mat.data == NULL) {
        printf("Memory allocation failed\n");
        return mat;
    }
    
    printf("Enter matrix elements row by row:\n");
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double value;
            printf("Element [%zu][%zu]: ", i, j);
            if (!read_safe_double(&value)) {
                printf("Invalid input\n");
                destroy_matrix(&mat);
                return create_matrix(0, 0);
            }
            set_matrix_element(&mat, i, j, value);
        }
    }
    return mat;
}

int main(void) {
    struct Matrix mat1 = create_matrix(0, 0);
    struct Matrix mat2 = create_matrix(0, 0);
    
    printf("Matrix Multiplication Program\n");
    printf("First matrix:\n");
    mat1 = read_matrix_input();
    if (mat1.data == NULL) {
        return 1;
    }
    
    printf("Second matrix:\n");
    mat2 = read_matrix_input();
    if (mat2.data == NULL) {
        destroy_matrix(&mat1);
        return 1;
    }
    
    struct Matrix result = matrix