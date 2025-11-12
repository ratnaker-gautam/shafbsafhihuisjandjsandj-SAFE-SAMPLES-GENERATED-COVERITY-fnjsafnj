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

int set_element(struct Matrix *mat, size_t row, size_t col, double value) {
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

double get_element(const struct Matrix *mat, size_t row, size_t col) {
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
                double a_val = get_element(a, i, k);
                double b_val = get_element(b, k, j);
                sum += a_val * b_val;
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

int read_positive_size(const char *prompt) {
    char buffer[32];
    int value = -1;
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    if (sscanf(buffer, "%d", &value) != 1) {
        return -1;
    }
    if (value <= 0 || value > 100) {
        return -1;
    }
    return value;
}

double read_double(const char *prompt) {
    char buffer[32];
    double value = 0.0;
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0.0;
    }
    if (sscanf(buffer, "%lf", &value) != 1) {
        return 0.0;
    }
    return value;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    rows1 = read_positive_size("Enter rows for matrix 1 (1-100): ");
    if (rows1 <= 0) {
        printf("Invalid rows for matrix 1\n");
        return 1;
    }
    cols1 = read_positive_size("Enter columns for matrix 1 (1-100): ");
    if (cols1 <= 0) {
        printf("Invalid columns for matrix 1\n");
        return 1;
    }
    rows2 = read_positive_size("Enter rows for matrix 2 (1-100): ");
    if (rows2 <= 0) {
        printf("Invalid rows for matrix 2\n");
        return 1;
    }
    cols2 = read_positive_size("Enter columns for matrix 2 (1-100): ");
    if (cols2 <= 0) {
        printf("Invalid columns for matrix 2\n");
        return 1;
    }
    if (cols