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
    if (mat && mat->data) {
        free(mat->data);
        mat->data = NULL;
    }
    mat->rows = 0;
    mat->cols = 0;
}

int set_matrix_element(struct Matrix *mat, size_t row, size_t col, double value) {
    if (!mat || !mat->data || row >= mat->rows || col >= mat->cols) {
        return 0;
    }
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_matrix_element(const struct Matrix *mat, size_t row, size_t col) {
    if (!mat || !mat->data || row >= mat->rows || col >= mat->cols) {
        return 0.0;
    }
    return mat->data[row * mat->cols + col];
}

struct MatrixResult multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result;
    result.success = 0;
    strcpy(result.error_msg, "");
    
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
    
    struct Matrix product = create_matrix(a->rows, b->cols);
    if (!product.data) {
        strcpy(result.error_msg, "Memory allocation failed");
        return result;
    }
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += get_matrix_element(a, i, k) * get_matrix_element(b, k, j);
            }
            set_matrix_element(&product, i, j, sum);
        }
    }
    
    result.success = 1;
    result.matrix = product;
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (!mat || !mat->data) {
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
    char buffer[64];
    printf("%s", prompt);
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    return endptr != buffer;
}

int read_size_t(const char *prompt, size_t *value) {
    char buffer[64];
    printf("%s", prompt);
    if (!fgets(buffer, sizeof(buffer), stdin)) {
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

int main() {
    struct Matrix a, b;
    size_t rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    
    if (!read_size_t("Enter rows for matrix A: ", &rows1) || rows1 == 0) {
        printf("Invalid rows for matrix A\n");
        return 1;
    }
    
    if (!read_size_t("Enter columns for matrix A: ", &cols1) || cols1 == 0) {
        printf("Invalid columns for matrix A\n");
        return 1;
    }
    
    a = create_matrix(rows1, cols1);
    if (!a.data) {
        printf("Failed to create matrix A\n");
        return 1;
    }
    
    printf("Enter elements for matrix A (%zux%zu):\n", rows1, cols1);
    for (size_t i = 0; i < rows1; i++) {
        for (size_t j = 0; j < cols1; j++) {
            double value;
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "A[%zu][%