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
};

struct Matrix create_matrix(size_t rows, size_t cols) {
    struct Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = NULL;
    if (rows > 0 && cols > 0 && rows <= SIZE_MAX / cols / sizeof(double)) {
        mat.data = calloc(rows * cols, sizeof(double));
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

int matrix_is_valid(const struct Matrix *mat) {
    return mat != NULL && mat->rows > 0 && mat->cols > 0 && mat->data != NULL;
}

struct MatrixResult matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    if (!matrix_is_valid(a) || !matrix_is_valid(b) || a->cols != b->rows) {
        return result;
    }
    result.matrix = create_matrix(a->rows, b->cols);
    if (!matrix_is_valid(&result.matrix)) {
        return result;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result.matrix.data[i * result.matrix.cols + j] = sum;
        }
    }
    result.success = 1;
    return result;
}

struct MatrixResult matrix_transpose(const struct Matrix *mat) {
    struct MatrixResult result = {0};
    if (!matrix_is_valid(mat)) {
        return result;
    }
    result.matrix = create_matrix(mat->cols, mat->rows);
    if (!matrix_is_valid(&result.matrix)) {
        return result;
    }
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            result.matrix.data[j * result.matrix.cols + i] = mat->data[i * mat->cols + j];
        }
    }
    result.success = 1;
    return result;
}

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    return endptr != buffer;
}

int read_size_t(const char *prompt, size_t *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    long long temp = strtoll(buffer, &endptr, 10);
    if (endptr == buffer || temp < 0 || temp > SIZE_MAX) {
        return 0;
    }
    *value = (size_t)temp;
    return 1;
}

struct Matrix read_matrix(void) {
    struct Matrix mat = {0};
    size_t rows, cols;
    if (!read_size_t("Enter number of rows: ", &rows) || !read_size_t("Enter number of columns: ", &cols)) {
        return mat;
    }
    if (rows == 0 || cols == 0 || rows > 100 || cols > 100) {
        return mat;
    }
    mat = create_matrix(rows, cols);
    if (!matrix_is_valid(&mat)) {
        return mat;
    }
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double value;
            printf("Enter element [%zu][%zu]: ", i, j);
            if (!read_double("", &value)) {
                destroy_matrix(&mat);
                return mat;
            }
            mat.data[i * cols + j] = value;
        }
    }
    return mat;
}

void print_matrix(const struct Matrix *mat) {
    if (!matrix_is_valid(mat)) {
        printf("Invalid matrix\n");
        return;
    }
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f ", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int main(void) {
    struct Matrix a = read_matrix();
    if (!matrix_is_valid(&a)) {
        printf("Failed to read matrix A\n");
        return 1;
    }
    struct Matrix b = read_matrix();
    if (!matrix_is_valid(&b)) {
        printf("Failed to read matrix B\n");
        destroy_matrix(&a);
        return 1;
    }
    printf("\nMatrix A:\n");
    print