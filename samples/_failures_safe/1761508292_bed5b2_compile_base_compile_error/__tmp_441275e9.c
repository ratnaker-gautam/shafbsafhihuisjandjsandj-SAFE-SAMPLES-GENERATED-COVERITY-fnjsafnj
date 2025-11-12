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

int read_positive_size(const char *prompt, size_t *value) {
    char buffer[32];
    long long temp;
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%lld", &temp) != 1) return 0;
    if (temp <= 0 || temp > 100) return 0;
    *value = (size_t)temp;
    return 1;
}

int read_double(const char *prompt, double *value) {
    char buffer[32];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%lf", value) != 1) return 0;
    return 1;
}

void input_matrix(struct Matrix *mat, const char *name) {
    if (mat == NULL || mat->data == NULL) return;
    printf("Enter values for matrix %s (%zux%zu):\n", name, mat->rows, mat->cols);
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double value;
            printf("Element [%zu][%zu]: ", i, j);
            if (!read_double("", &value)) {
                printf("Invalid input, using 0.0\n");
                value = 0.0;
            }
            set_matrix_element(mat, i, j, value);
        }
    }
}

int main(void) {
    struct Matrix a, b, result;
    size_t rows1, cols1, rows2, cols2;

    printf("Matrix Multiplication Program\n");
    printf("Matrix dimensions must be between 1 and 100\n");

    if (!read_positive_size("Enter rows for matrix A: ", &rows1)) {
        printf("Invalid rows for matrix A\n");
        return 1;
    }
    if (!read_positive_size("Enter columns for matrix A: ", &cols1)) {
        printf("Invalid columns for matrix A\n");
        return 1;
    }

    if (!read_positive_size("Enter rows for matrix B: ", &rows2)) {
        printf("Invalid rows for matrix B\n");
        return 1;
    }
    if (!read_positive_size("Enter columns for matrix B: ", &cols