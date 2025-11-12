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
    size_t total = rows * cols;
    if (total > SIZE_MAX / sizeof(double)) {
        return mat;
    }
    mat.data = malloc(total * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
        return mat;
    }
    for (size_t i = 0; i < total; i++) {
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

int set_matrix_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (mat == NULL || mat->data == NULL) {
        return 0;
    }
    if (row >= mat->rows || col >= mat->cols) {
        return 0;
    }
    size_t index = row * mat->cols + col;
    mat->data[index] = value;
    return 1;
}

double get_matrix_element(const struct Matrix *mat, size_t row, size_t col) {
    if (mat == NULL || mat->data == NULL) {
        return 0.0;
    }
    if (row >= mat->rows || col >= mat->cols) {
        return 0.0;
    }
    size_t index = row * mat->cols + col;
    return mat->data[index];
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
                double val_a = get_matrix_element(a, i, k);
                double val_b = get_matrix_element(b, k, j);
                sum += val_a * val_b;
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

int read_safe_size_t(size_t *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    unsigned long long temp = strtoull(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    if (temp > SIZE_MAX) {
        return 0;
    }
    *value = (size_t)temp;
    return 1;
}

double read_safe_double() {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0.0;
    }
    char *endptr;
    double value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0.0;
    }
    return value;
}

int main() {
    struct Matrix a, b, result;
    a.rows = 0; a.cols = 0; a.data = NULL;
    b.rows = 0; b.cols = 0; b.data = NULL;
    result.rows = 0; result.cols = 0; result.data = NULL;

    printf("Enter rows for matrix A: ");
    size_t rows_a;
    if (!read_safe_size_t(&rows_a) || rows_a == 0) {
        printf("Invalid input for rows\n");
        return 1;
    }

    printf("Enter columns for matrix A: ");
    size_t cols_a;
    if (!read_safe_size_t(&cols_a) || cols_a == 0) {
        printf("Invalid input for columns\n");
        return 1;
    }

    a = create_matrix(rows_a, cols_a);
    if (a.data == NULL) {
        printf("Failed to