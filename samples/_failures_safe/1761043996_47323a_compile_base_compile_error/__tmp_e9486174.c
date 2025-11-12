//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
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
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols / sizeof(double)) {
        mat.data = NULL;
        return mat;
    }
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
        return mat;
    }
    memset(mat.data, 0, rows * cols * sizeof(double));
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
    if (mat == NULL || mat->data == NULL) return 0;
    if (row >= mat->rows || col >= mat->cols) return 0;
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_element(const struct Matrix *mat, size_t row, size_t col) {
    if (mat == NULL || mat->data == NULL) return 0.0;
    if (row >= mat->rows || col >= mat->cols) return 0.0;
    return mat->data[row * mat->cols + col];
}

struct Matrix multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
    if (a->cols != b->rows) return result;
    result = create_matrix(a->rows, b->cols);
    if (result.data == NULL) return result;
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
    if (mat == NULL || mat->data == NULL) return;
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", get_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_matrix(struct Matrix *mat) {
    if (mat == NULL) return 0;
    printf("Enter matrix elements row by row:\n");
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double value;
            if (scanf("%lf", &value) != 1) return 0;
            set_element(mat, i, j, value);
        }
    }
    return 1;
}

int main(void) {
    struct Matrix a, b, result;
    size_t rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%zu %zu", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows1 == 0 || cols1 == 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    a = create_matrix(rows1, cols1);
    if (a.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter dimensions of second matrix (rows cols): ");
    if (scanf("%zu %zu", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        destroy_matrix(&a);
        return 1;
    }
    if (rows2 == 0 || cols2 == 0) {
        printf("Invalid dimensions\n");
        destroy_matrix(&a);
        return 1;
    }
    
    b = create_matrix(rows2, cols2);
    if (b.data == NULL) {
        printf("Memory allocation failed\n");
        destroy_matrix(&a);
        return 1;
    }
    
    printf("First matrix:\n");
    if (!read_matrix(&a)) {
        printf("Failed to read matrix\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    
    printf("Second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Failed to read matrix\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    
    result = multiply_matrices(&a, &b);
    if (result.data == NULL) {
        printf("Matrix multiplication failed\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1