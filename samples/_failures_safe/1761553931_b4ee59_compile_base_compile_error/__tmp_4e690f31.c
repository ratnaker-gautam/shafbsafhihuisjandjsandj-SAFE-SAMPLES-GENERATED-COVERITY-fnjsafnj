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

int read_double(const char *prompt, double *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) return 0;
    return 1;
}

int read_size_t(const char *prompt, size_t *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long long temp = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || temp < 0 || temp > SIZE_MAX) return 0;
    *value = (size_t)temp;
    return 1;
}

int main(void) {
    struct Matrix a, b, result;
    size_t rows1, cols1, rows2, cols2;
    
    if (!read_size_t("Enter rows for matrix A: ", &rows1)) {
        printf("Invalid input for rows\n");
        return 1;
    }
    if (!read_size_t("Enter columns for matrix A: ", &cols1)) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    a = create_matrix(rows1, cols1);
    if (a.data == NULL) {
        printf("Failed to create matrix A\n");
        return 1;
    }
    
    printf("Enter elements for matrix A (%zu x %zu):\n", rows1, cols1);
    for (size_t i = 0; i < rows1; i++) {
        for (size_t j = 0; j < cols1; j++) {
            double value;
            printf("A[%zu][%zu]: ", i, j);
            if (!read_double("", &value)) {
                printf("Invalid input\n");
                destroy_matrix(&a);
                return 1;
            }
            set_matrix_element(&a, i, j, value);
        }
    }
    
    if (!read_size_t("Enter rows for matrix B: ", &rows2)) {
        printf("Invalid input for rows\n