//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
                sum += get_element(a, i, k) * get_element(b, k, j);
            }
            set_element(&result, i, j, sum);
        }
    }
    return result;
}

struct Matrix matrix_transpose(const struct Matrix *mat) {
    struct Matrix result = create_matrix(0, 0);
    if (mat == NULL || mat->data == NULL) return result;
    
    result = create_matrix(mat->cols, mat->rows);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            set_element(&result, j, i, get_element(mat, i, j));
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
    if (mat == NULL || mat->data == NULL) return 0;
    printf("Enter %zu x %zu matrix elements:\n", mat->rows, mat->cols);
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
    size_t rows1, cols1, rows2, cols2;
    
    printf("Enter first matrix dimensions (rows cols): ");
    if (scanf("%zu %zu", &rows1, &cols1) != 2 || rows1 == 0 || cols1 == 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    struct Matrix mat1 = create_matrix(rows1, cols1);
    if (mat1.data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!read_matrix(&mat1)) {
        printf("Failed to read matrix\n");
        destroy_matrix(&mat1);
        return 1;
    }
    
    printf("Enter second matrix dimensions (rows cols): ");
    if (scanf("%zu %zu", &rows2, &cols2) != 2 || rows2 == 0 || cols2 == 0) {
        printf("Invalid dimensions\n");
        destroy_matrix(&mat1);
        return 1;
    }
    
    struct Matrix mat2 = create_matrix(rows2, cols2);
    if (mat2.data == NULL) {
        printf("Memory allocation failed\n");
        destroy_matrix(&mat1);
        return 1;
    }
    
    if (!read_matrix(&mat2)) {
        printf("Failed to read matrix\n");
        destroy_matrix(&mat1);
        destroy_matrix(&mat2);
        return 1;
    }
    
    printf("\nMatrix 1:\n");