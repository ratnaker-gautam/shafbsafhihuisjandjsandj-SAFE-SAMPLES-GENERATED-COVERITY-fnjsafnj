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

int initialize_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return 0;
    
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        mat->data[i] = (i % 10) + 1.0;
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_norm(struct Matrix *mat) {
    if (!mat || !mat->data) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int multiply_matrices(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    return 1;
}

int main() {
    printf("Matrix Operations Demo\n");
    
    struct Matrix *mat1 = create_matrix(2, 2);
    if (!mat1) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    
    struct Matrix *mat2 = create_matrix(2, 2);
    if (!mat2) {
        printf("Failed to create matrix 2\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    struct Matrix *result = create_matrix(2, 2);
    if (!result) {
        printf("Failed to create result matrix\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        return 1;
    }
    
    if (!initialize_matrix(mat1) || !initialize_matrix(mat2)) {
        printf("Failed to initialize matrices\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        destroy_matrix(result);
        return 1;
    }
    
    double det1 = compute_determinant_2x2(mat1);
    double det2 = compute_determinant_2x2(mat2);
    
    printf("Matrix 1 determinant: %.2f\n", det1);
    printf("Matrix 2 determinant: %.2f\n", det2);
    
    double norm1 = compute_matrix_norm(mat1);
    double norm2 = compute_matrix_norm(mat2);
    
    printf("Matrix 1 norm: %.2f\n", norm1);
    printf("Matrix 2 norm: %.2f\n", norm2);
    
    if (multiply_matrices(mat1, mat2, result)) {
        double result_norm = compute_matrix_norm(result);
        printf("Product matrix norm: %.2f\n", result_norm);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    double stack_matrix[4] = {1.5, 2.5, 3.5, 4.5};
    struct Matrix stack_mat = {stack_matrix, 2, 2};
    double stack_det = compute_determinant_2x2(&stack_mat);
    double stack_norm = compute_matrix_norm(&stack_mat);
    
    printf("Stack matrix determinant: %.2f\n", stack_det);
    printf("Stack matrix norm: %.2f\n", stack_norm);
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(result);
    
    return 0;
}