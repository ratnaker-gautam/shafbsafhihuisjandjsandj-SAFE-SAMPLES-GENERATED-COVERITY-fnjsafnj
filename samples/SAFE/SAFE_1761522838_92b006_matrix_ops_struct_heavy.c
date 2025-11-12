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
    struct Matrix mat = {0, 0, NULL};
    if (rows == 0 || cols == 0) return mat;
    if (rows > SIZE_MAX / cols / sizeof(double)) return mat;
    
    mat.data = malloc(rows * cols * sizeof(double));
    if (mat.data == NULL) return mat;
    
    mat.rows = rows;
    mat.cols = cols;
    memset(mat.data, 0, rows * cols * sizeof(double));
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

struct MatrixResult multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    struct MatrixResult result = {0, {0, 0, NULL}, ""};
    
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        strcpy(result.error_msg, "Invalid input matrices");
        return result;
    }
    
    if (a->cols != b->rows) {
        strcpy(result.error_msg, "Dimension mismatch for multiplication");
        return result;
    }
    
    struct Matrix product = create_matrix(a->rows, b->cols);
    if (product.data == NULL) {
        strcpy(result.error_msg, "Memory allocation failed");
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

struct MatrixResult read_matrix_from_input(void) {
    struct MatrixResult result = {0, {0, 0, NULL}, ""};
    size_t rows, cols;
    
    printf("Enter number of rows: ");
    if (scanf("%zu", &rows) != 1 || rows == 0 || rows > 100) {
        strcpy(result.error_msg, "Invalid row count");
        return result;
    }
    
    printf("Enter number of columns: ");
    if (scanf("%zu", &cols) != 1 || cols == 0 || cols > 100) {
        strcpy(result.error_msg, "Invalid column count");
        return result;
    }
    
    struct Matrix mat = create_matrix(rows, cols);
    if (mat.data == NULL) {
        strcpy(result.error_msg, "Memory allocation failed");
        return result;
    }
    
    printf("Enter matrix elements row by row:\n");
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (scanf("%lf", &mat.data[i * cols + j]) != 1) {
                strcpy(result.error_msg, "Invalid matrix element");
                destroy_matrix(&mat);
                return result;
            }
        }
    }
    
    result.success = 1;
    result.matrix = mat;
    return result;
}

void print_matrix(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) return;
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%8.2f ", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int main(void) {
    printf("Matrix Multiplication Program\n");
    printf("First matrix:\n");
    struct MatrixResult mat1_result = read_matrix_from_input();
    if (!mat1_result.success) {
        printf("Error: %s\n", mat1_result.error_msg);
        return 1;
    }
    
    printf("Second matrix:\n");
    struct MatrixResult mat2_result = read_matrix_from_input();
    if (!mat2_result.success) {
        printf("Error: %s\n", mat2_result.error_msg);
        destroy_matrix(&mat1_result.matrix);
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1_result.matrix);
    printf("\nSecond matrix:\n");
    print_matrix(&mat2_result.matrix);
    
    struct MatrixResult product_result = multiply_matrices(&mat1_result.matrix, &mat2_result.matrix);
    
    if (product_result.success) {
        printf("\nProduct matrix:\n");
        print_matrix(&product_result.matrix);
        destroy_matrix(&product_result.matrix);
    } else {
        printf("Error: %s\n", product_result.error_msg);
    }
    
    destroy_matrix(&mat1_result.matrix);
    destroy_matrix(&mat2_result.matrix);
    
    return 0;
}