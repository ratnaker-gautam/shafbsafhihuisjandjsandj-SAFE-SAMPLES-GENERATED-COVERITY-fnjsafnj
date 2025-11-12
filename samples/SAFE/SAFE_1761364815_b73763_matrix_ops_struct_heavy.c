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
    mat->data[row * mat->cols + col] = value;
    return 1;
}

double get_element(const struct Matrix *mat, size_t row, size_t col) {
    if (mat == NULL || mat->data == NULL) {
        return 0.0;
    }
    if (row >= mat->rows || col >= mat->cols) {
        return 0.0;
    }
    return mat->data[row * mat->cols + col];
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
                sum += get_element(a, i, k) * get_element(b, k, j);
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
    struct Matrix mat1 = create_matrix(2, 3);
    struct Matrix mat2 = create_matrix(3, 2);
    
    if (mat1.data == NULL || mat2.data == NULL) {
        printf("Failed to create matrices\n");
        destroy_matrix(&mat1);
        destroy_matrix(&mat2);
        return 1;
    }
    
    set_element(&mat1, 0, 0, 1.0);
    set_element(&mat1, 0, 1, 2.0);
    set_element(&mat1, 0, 2, 3.0);
    set_element(&mat1, 1, 0, 4.0);
    set_element(&mat1, 1, 1, 5.0);
    set_element(&mat1, 1, 2, 6.0);
    
    set_element(&mat2, 0, 0, 7.0);
    set_element(&mat2, 0, 1, 8.0);
    set_element(&mat2, 1, 0, 9.0);
    set_element(&mat2, 1, 1, 10.0);
    set_element(&mat2, 2, 0, 11.0);
    set_element(&mat2, 2, 1, 12.0);
    
    printf("Matrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    struct Matrix result = multiply_matrices(&mat1, &mat2);
    
    if (result.data != NULL) {
        printf("\nResult of multiplication:\n");
        print_matrix(&result);
        destroy_matrix(&result);
    } else {
        printf("\nMatrix multiplication failed\n");
    }
    
    destroy_matrix(&mat1);
    destroy_matrix(&mat2);
    
    return 0;
}