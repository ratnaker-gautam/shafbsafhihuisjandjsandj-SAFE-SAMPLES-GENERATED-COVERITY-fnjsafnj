//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
};

struct Matrix create_matrix(size_t rows, size_t cols) {
    struct Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) {
        mat.rows = 0;
        mat.cols = 0;
    }
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat != NULL && mat->data != NULL) {
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

struct Matrix matrix_transpose(const struct Matrix *mat) {
    struct Matrix result = create_matrix(0, 0);
    if (mat == NULL || mat->data == NULL) return result;
    
    result = create_matrix(mat->cols, mat->rows);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            set_matrix_element(&result, j, i, get_matrix_element(mat, i, j));
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

int read_double(double *value) {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    return (endptr != buffer);
}

int main(void) {
    struct Matrix a = create_matrix(0, 0);
    struct Matrix b = create_matrix(0, 0);
    
    printf("Enter dimensions for matrix A (rows cols): ");
    size_t rows_a, cols_a;
    if (scanf("%zu %zu", &rows_a, &cols_a) != 2 || rows_a == 0 || cols_a == 0) {
        printf("Invalid dimensions for matrix A\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    getchar();
    
    a = create_matrix(rows_a, cols_a);
    if (a.data == NULL) {
        printf("Memory allocation failed for matrix A\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    
    printf("Enter %zu elements for matrix A (row-major):\n", rows_a * cols_a);
    for (size_t i = 0; i < rows_a; i++) {
        for (size_t j = 0; j < cols_a; j++) {
            double value;
            if (!read_double(&value)) {
                printf("Invalid input for matrix element\n");
                destroy_matrix(&a);
                destroy_matrix(&b);
                return 1;
            }
            set_matrix_element(&a, i, j, value);
        }
    }
    
    printf("Enter dimensions for matrix B (rows cols): ");
    size_t rows_b, cols_b;
    if (scanf("%zu %zu", &rows_b, &cols_b) != 2 || rows_b == 0 || cols_b == 0) {
        printf("Invalid dimensions for matrix B\n");
        destroy_matrix(&a);
        destroy_matrix(&b);
        return 1;
    }
    getchar();