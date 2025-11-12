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
    if (rows == 0 || cols == 0) {
        mat.data = NULL;
        return mat;
    }
    if (SIZE_MAX / rows < cols) {
        mat.data = NULL;
        return mat;
    }
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
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

int read_matrix_from_input(struct Matrix *mat) {
    if (mat == NULL) return 0;
    printf("Enter matrix dimensions (rows cols): ");
    size_t rows, cols;
    if (scanf("%zu %zu", &rows, &cols) != 2) return 0;
    if (rows == 0 || cols == 0) return 0;
    if (rows > 100 || cols > 100) return 0;
    *mat = create_matrix(rows, cols);
    if (mat->data == NULL) return 0;
    printf("Enter %zu elements row by row:\n", rows * cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double value;
            if (scanf("%lf", &value) != 1) {
                destroy_matrix(mat);
                return 0;
            }
            set_matrix_element(mat, i, j, value);
        }
    }
    return 1;
}

int main(void) {
    struct Matrix mat1, mat2;
    printf("Matrix Multiplication Program\n");
    printf("First matrix:\n");
    if (!read_matrix_from_input(&mat1)) {
        printf("Invalid first matrix input.\n");
        return 1;
    }
    printf("Second matrix:\n");
    if (!read_matrix_from_input(&mat2)) {
        printf("Invalid second matrix input.\n");
        destroy_matrix(&mat1);
        return 1;
    }
    struct Matrix result = matrix_multiply(&mat1, &mat2);
    if (result.data == NULL) {
        printf("Matrix multiplication failed.\n");
    } else {
        printf("Result matrix:\n");
        print_matrix(&result);
        destroy_matrix(&result);
    }
    destroy_matrix(&mat1);
    destroy_matrix(&mat2);
    return 0;
}