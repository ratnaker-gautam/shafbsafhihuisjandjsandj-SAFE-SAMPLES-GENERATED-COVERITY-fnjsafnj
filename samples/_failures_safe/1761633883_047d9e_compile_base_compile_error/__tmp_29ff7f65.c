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
    destroy_matrix(&result);
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

int read_safe_size(const char *prompt, size_t *value) {
    char buffer[32];
    long long temp;
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%lld", &temp) != 1) return 0;
    if (temp < 1 || temp > 1000) return 0;
    *value = (size_t)temp;
    return 1;
}

int read_safe_double(const char *prompt, double *value) {
    char buffer[32];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%lf", value) != 1) return 0;
    return 1;
}

void input_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double value;
            printf("Element [%zu][%zu]: ", i, j);
            if (!read_safe_double("", &value)) {
                printf("Invalid input, using 0.0\n");
                value = 0.0;
            }
            set_element(mat, i, j, value);
        }
    }
}

int main(void) {
    struct Matrix a, b, result;
    size_t rows1, cols1, rows2, cols2;

    printf("Matrix Multiplication Program\n");
    printf("Enter dimensions for first matrix:\n");
    if (!read_safe_size("Rows (1-1000): ", &rows1)) {
        printf("Invalid rows input\n");
        return 1;
    }
    if (!read_safe_size("Cols (1-1000): ", &cols1)) {
        printf("Invalid cols input\n");
        return 1;
    }

    printf("Enter dimensions for second matrix:\n");
    if (!read_safe_size("Rows (1-1000): ", &rows2)) {
        printf("Invalid rows input\n");
        return 1;
    }
    if (!read_safe_size("Cols (1-1000): ", &cols2)) {
        printf("Invalid cols input\n");
        return 1;
    }