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
    
    if (rows == 0 || cols == 0) {
        return mat;
    }
    
    if (rows > SIZE_MAX / cols / sizeof(double)) {
        return mat;
    }
    
    mat.data = malloc(rows * cols * sizeof(double));
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

struct MatrixResult multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0};
    
    if (a == NULL || b == NULL) {
        strncpy(result.error_msg, "Null matrix input", sizeof(result.error_msg) - 1);
        return result;
    }
    
    if (a->data == NULL || b->data == NULL) {
        strncpy(result.error_msg, "Matrix data is null", sizeof(result.error_msg) - 1);
        return result;
    }
    
    if (a->cols != b->rows) {
        strncpy(result.error_msg, "Dimension mismatch for multiplication", sizeof(result.error_msg) - 1);
        return result;
    }
    
    struct Matrix product = create_matrix(a->rows, b->cols);
    if (product.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg) - 1);
        return result;
    }
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            product.data[i * product.cols + j] = sum;
        }
    }
    
    result.success = 1;
    result.matrix = product;
    return result;
}

struct MatrixResult read_matrix_input(void) {
    struct MatrixResult result = {0};
    size_t rows, cols;
    
    printf("Enter number of rows: ");
    if (scanf("%zu", &rows) != 1) {
        strncpy(result.error_msg, "Invalid row input", sizeof(result.error_msg) - 1);
        return result;
    }
    
    printf("Enter number of columns: ");
    if (scanf("%zu", &cols) != 1) {
        strncpy(result.error_msg, "Invalid column input", sizeof(result.error_msg) - 1);
        return result;
    }
    
    if (rows == 0 || cols == 0) {
        strncpy(result.error_msg, "Dimensions must be positive", sizeof(result.error_msg) - 1);
        return result;
    }
    
    struct Matrix mat = create_matrix(rows, cols);
    if (mat.data == NULL) {
        strncpy(result.error_msg, "Memory allocation failed", sizeof(result.error_msg) - 1);
        return result;
    }
    
    printf("Enter %zu matrix elements row by row:\n", rows * cols);
    for (size_t i = 0; i < rows * cols; i++) {
        if (scanf("%lf", &mat.data[i]) != 1) {
            destroy_matrix(&mat);
            strncpy(result.error_msg, "Invalid matrix element", sizeof(result.error_msg) - 1);
            return result;
        }
    }
    
    result.success = 1;
    result.matrix = mat;
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
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
    printf("Matrix Multiplication Program\n");
    printf("=============================\n");
    
    printf("First matrix:\n");
    struct MatrixResult mat1_result = read_matrix_input();
    if (!mat1_result.success) {
        printf("Error: %s\n", mat1_result.error_msg);
        return 1;
    }
    
    printf("Second matrix:\n");
    struct MatrixResult mat2_result = read_matrix_input();
    if (!mat2_result.success) {
        printf("Error: %s\n", mat2_result.error_msg);
        destroy_matrix(&mat1_result.matrix);
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1_result.matrix);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2_result.matrix);
    
    struct MatrixResult product_result =