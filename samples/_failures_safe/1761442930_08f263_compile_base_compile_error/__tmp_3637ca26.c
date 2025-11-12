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
    if (rows == 0 || cols == 0) return mat;
    if (SIZE_MAX / cols < rows) return mat;
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) return mat;
    for (size_t i = 0; i < rows * cols; i++) {
        mat.data[i] = 0.0;
    }
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat == NULL) return;
    free(mat->data);
    mat->data = NULL;
    mat->rows = 0;
    mat->cols = 0;
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

struct Matrix add_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) return result;
    if (a->rows != b->rows || a->cols != b->cols) return result;
    result = create_matrix(a->rows, a->cols);
    if (result.data == NULL) return result;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            double val = get_matrix_element(a, i, j) + get_matrix_element(b, i, j);
            set_matrix_element(&result, i, j, val);
        }
    }
    return result;
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
                sum += get_matrix_element(a, i, k) * get_matrix_element(b, k, j);
            }
            set_matrix_element(&result, i, j, sum);
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
            printf("%8.2f", get_matrix_element(mat, i, j));
        }
        printf("\n");
    }
}

int read_double(double *value) {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) return 0;
    return 1;
}

int main() {
    struct Matrix mat1 = create_matrix(0, 0);
    struct Matrix mat2 = create_matrix(0, 0);
    struct Matrix result = create_matrix(0, 0);
    
    printf("Enter dimensions for first matrix (rows cols): ");
    size_t r1, c1;
    if (scanf("%zu %zu", &r1, &c1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    mat1 = create_matrix(r1, c1);
    if (mat1.data == NULL) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    
    printf("Enter %zu elements for matrix 1:\n", r1 * c1);
    for (size_t i = 0; i < r1; i++) {
        for (size_t j = 0; j < c1; j++) {
            double val;
            if (!read_double(&val)) {
                printf("Invalid input\n");
                destroy_matrix(&mat1);
                return 1;
            }
            set_matrix_element(&mat1, i, j, val);
        }
    }
    
    printf("