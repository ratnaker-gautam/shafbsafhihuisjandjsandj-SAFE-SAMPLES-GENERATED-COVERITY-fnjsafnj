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
    if (rows > 0 && cols > 0 && SIZE_MAX / rows >= cols) {
        mat.data = malloc(rows * cols * sizeof(double));
        if (mat.data != NULL) {
            memset(mat.data, 0, rows * cols * sizeof(double));
        }
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

int get_matrix_element(const struct Matrix *mat, size_t row, size_t col, double *value) {
    if (!matrix_is_valid(mat) || row >= mat->rows || col >= mat->cols || value == NULL) {
        return 0;
    }
    *value = mat->data[row * mat->cols + col];
    return 1;
}

int set_matrix_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (!matrix_is_valid(mat) || row >= mat->rows || col >= mat->cols) {
        return 0;
    }
    mat->data[row * mat->cols + col] = value;
    return 1;
}

struct Matrix matrix_multiply(const struct Matrix *a, const struct Matrix *b) {
    struct Matrix result = create_matrix(0, 0);
    if (!matrix_is_valid(a) || !matrix_is_valid(b) || a->cols != b->rows) {
        return result;
    }
    result = create_matrix(a->rows, b->cols);
    if (!matrix_is_valid(&result)) {
        return result;
    }
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                double a_val, b_val;
                if (get_matrix_element(a, i, k, &a_val) && get_matrix_element(b, k, j, &b_val)) {
                    sum += a_val * b_val;
                }
            }
            set_matrix_element(&result, i, j, sum);
        }
    }
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (!matrix_is_valid(mat)) {
        printf("Invalid matrix\n");
        return;
    }
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            double value;
            if (get_matrix_element(mat, i, j, &value)) {
                printf("%8.2f ", value);
            }
        }
        printf("\n");
    }
}

int read_double(const char *prompt, double *value) {
    if (prompt == NULL || value == NULL) {
        return 0;
    }
    printf("%s", prompt);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    return endptr != buffer;
}

int read_size_t(const char *prompt, size_t *value) {
    if (prompt == NULL || value == NULL) {
        return 0;
    }
    printf("%s", prompt);
    char buffer[256];
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
    struct Matrix mat = create_matrix(0, 0);
    size_t rows, cols;
    if (!read_size_t("Enter number of rows: ", &rows) || rows == 0) {
        return mat;
    }
    if (!read_size_t("Enter number of columns: ", &cols) || cols == 0) {
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
                return create_matrix(0, 0);
            }
            set_matrix_element(&mat, i, j, value);
        }
    }
    return mat;
}

int main(void) {
    struct Matrix a = read_matrix();
    if (!matrix_is_valid(&a