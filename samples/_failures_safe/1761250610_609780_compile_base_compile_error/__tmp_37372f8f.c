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

struct MatrixResult {
    int success;
    struct Matrix matrix;
    char error_msg[64];
};

struct MatrixInput {
    size_t rows;
    size_t cols;
    double *values;
};

struct Matrix create_matrix(size_t rows, size_t cols) {
    struct Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = NULL;
    if (rows > 0 && cols > 0 && rows <= SIZE_MAX / cols / sizeof(double)) {
        mat.data = (double*)calloc(rows * cols, sizeof(double));
    }
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat && mat->data) {
        free(mat->data);
        mat->data = NULL;
    }
    mat->rows = 0;
    mat->cols = 0;
}

struct MatrixResult multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    if (!a || !b || !a->data || !b->data) {
        strcpy(result.error_msg, "Invalid input matrices");
        return result;
    }
    if (a->cols != b->rows) {
        strcpy(result.error_msg, "Dimension mismatch for multiplication");
        return result;
    }
    if (a->rows == 0 || a->cols == 0 || b->rows == 0 || b->cols == 0) {
        strcpy(result.error_msg, "Empty matrix");
        return result;
    }
    if (a->rows > SIZE_MAX / b->cols / sizeof(double)) {
        strcpy(result.error_msg, "Result matrix too large");
        return result;
    }
    result.matrix = create_matrix(a->rows, b->cols);
    if (!result.matrix.data) {
        strcpy(result.error_msg, "Memory allocation failed");
        return result;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result.matrix.data[i * b->cols + j] = sum;
        }
    }
    result.success = 1;
    return result;
}

struct MatrixResult add_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    if (!a || !b || !a->data || !b->data) {
        strcpy(result.error_msg, "Invalid input matrices");
        return result;
    }
    if (a->rows != b->rows || a->cols != b->cols) {
        strcpy(result.error_msg, "Dimension mismatch for addition");
        return result;
    }
    if (a->rows == 0 || a->cols == 0) {
        strcpy(result.error_msg, "Empty matrix");
        return result;
    }
    result.matrix = create_matrix(a->rows, a->cols);
    if (!result.matrix.data) {
        strcpy(result.error_msg, "Memory allocation failed");
        return result;
    }
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result.matrix.data[i] = a->data[i] + b->data[i];
    }
    result.success = 1;
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (!mat || !mat->data) return;
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int read_double(const char *prompt, double *value) {
    char buffer[64];
    printf("%s", prompt);
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    return (endptr != buffer);
}

int read_size_t(const char *prompt, size_t *value) {
    char buffer[64];
    printf("%s", prompt);
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    char *endptr;
    long long temp = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || temp < 0 || temp > SIZE_MAX) return 0;
    *value = (size_t)temp;
    return 1;
}

struct Matrix read_matrix_interactive() {
    struct Matrix mat = {0};
    size_t rows, cols;
    if (!read_size_t("Enter number of rows: ", &rows) || rows == 0) return mat;
    if (!read_size_t("Enter number of columns: ", &cols) || cols == 0) return mat;
    mat = create_matrix(rows, cols);
    if (!mat.data) return mat;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j