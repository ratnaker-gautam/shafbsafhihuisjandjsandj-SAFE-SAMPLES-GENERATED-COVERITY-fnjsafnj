//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 100

struct Matrix {
    double *data;
    int rows;
    int cols;
};

int validate_matrix_params(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_params(rows, cols)) return NULL;
    
    struct Matrix *mat = malloc(sizeof(struct Matrix));
    if (!mat) return NULL;
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * cols * sizeof(double));
    
    if (!mat->data) {
        free(mat);
        return NULL;
    }
    
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat) {
        free(mat->data);
        free(mat);
    }
}

int initialize_matrix(struct Matrix *mat, double values[], int count) {
    if (!mat || !values) return 0;
    if (count != mat->rows * mat->cols) return 0;
    
    for (int i = 0; i < count; i++) {
        mat->data[i] = values[i];
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_norm(struct Matrix *mat) {
    if (!mat) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int main() {
    double stack_values[4] = {3.0, 8.0, 4.0, 6.0};
    double heap_values[4] = {1.5, 2.5, 3.5, 4.5};
    
    struct Matrix *mat1 = create_matrix(2, 2);
    if (!mat1) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    
    if (!initialize_matrix(mat1, stack_values, 4)) {
        printf("Failed to initialize matrix 1\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    struct Matrix *mat2 = create_matrix(2, 2);
    if (!mat2) {
        printf("Failed to create matrix 2\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    if (!initialize_matrix(mat2, heap_values, 4)) {
        printf("Failed to initialize matrix 2\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    double det1 = compute_determinant_2x2(mat1);
    double det2 = compute_determinant_2x2(mat2);
    
    double norm1 = compute_matrix_norm(mat1);
    double norm2 = compute_matrix_norm(mat2);
    
    printf("Matrix 1 determinant: %.2f\n", det1);
    printf("Matrix 2 determinant: %.2f\n", det2);
    printf("Matrix 1 norm: %.2f\n", norm1);
    printf("Matrix 2 norm: %.2f\n", norm2);
    
    struct Matrix stack_mat;
    stack_mat.rows = 2;
    stack_mat.cols = 2;
    double stack_data[4] = {2.0, 1.0, 1.0, 2.0};
    stack_mat.data = stack_data;
    
    double stack_det = compute_determinant_2x2(&stack_mat);
    double stack_norm = compute_matrix_norm(&stack_mat);
    
    printf("Stack matrix determinant: %.2f\n", stack_det);
    printf("Stack matrix norm: %.2f\n", stack_norm);
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    
    return 0;
}