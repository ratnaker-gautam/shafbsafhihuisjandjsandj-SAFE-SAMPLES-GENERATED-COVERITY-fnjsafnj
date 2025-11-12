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
                double aval = get_element(a, i, k);
                double bval = get_element(b, k, j);
                sum += aval * bval;
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

int main(void) {
    struct Matrix a = create_matrix(2, 3);
    struct Matrix b = create_matrix(3, 2);
    if (a.data == NULL || b.data == NULL) {
        printf("Matrix creation failed\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    set_element(&a, 0, 0, 1.0);
    set_element(&a, 0, 1, 2.0);
    set_element(&a, 0, 2, 3.0);
    set_element(&a, 1, 0, 4.0);
    set_element(&a, 1, 1, 5.0);
    set_element(&a, 1, 2, 6.0);
    set_element(&b, 0, 0, 7.0);
    set_element(&b, 0, 1, 8.0);
    set_element(&b, 1, 0, 9.0);
    set_element(&b, 1, 1, 10.0);
    set_element(&b, 2, 0, 11.0);
    set_element(&b, 2, 1, 12.0);
    printf("Matrix A:\n");
    print_matrix(&a);
    printf("Matrix B:\n");
    print_matrix(&b);
    struct Matrix c = multiply_matrices(&a, &b);
    if (c.data == NULL) {
        printf("Matrix multiplication failed\n");
    } else {
        printf("Result A * B:\n");
        print_matrix(&c);
        destroy_matrix(&c);
    }
    destroy_matrix(&a);
    destroy_matrix(&b);
    return 0;
}