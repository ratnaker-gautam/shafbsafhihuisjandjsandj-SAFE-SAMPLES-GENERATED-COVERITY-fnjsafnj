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

int initialize_matrix(struct Matrix *mat, double values[]) {
    if (!mat || !values) return 0;
    
    for (int i = 0; i < mat->rows * mat->cols; i++) {
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
    
    if (sum < 0.0) return 0.0;
    return sqrt(sum);
}

int main() {
    double stack_values[4] = {3.0, 8.0, 4.0, 6.0};
    
    struct Matrix *heap_matrix = create_matrix(2, 2);
    if (!heap_matrix) {
        printf("Failed to create matrix\n");
        return 1;
    }
    
    if (!initialize_matrix(heap_matrix, stack_values)) {
        printf("Failed to initialize matrix\n");
        destroy_matrix(heap_matrix);
        return 1;
    }
    
    double det = compute_determinant_2x2(heap_matrix);
    double norm = compute_matrix_norm(heap_matrix);
    
    printf("Matrix data:\n");
    for (int i = 0; i < heap_matrix->rows; i++) {
        for (int j = 0; j < heap_matrix->cols; j++) {
            printf("%.2f ", heap_matrix->data[i * heap_matrix->cols + j]);
        }
        printf("\n");
    }
    
    printf("Determinant: %.2f\n", det);
    printf("Frobenius norm: %.2f\n", norm);
    
    struct Matrix stack_matrix;
    stack_matrix.rows = 2;
    stack_matrix.cols = 2;
    double stack_data[4] = {1.5, 2.5, 3.5, 4.5};
    stack_matrix.data = stack_data;
    
    double stack_det = compute_determinant_2x2(&stack_matrix);
    double stack_norm = compute_matrix_norm(&stack_matrix);
    
    printf("\nStack matrix:\n");
    for (int i = 0; i < stack_matrix.rows; i++) {
        for (int j = 0; j < stack_matrix.cols; j++) {
            printf("%.2f ", stack_matrix.data[i * stack_matrix.cols + j]);
        }
        printf("\n");
    }
    
    printf("Stack determinant: %.2f\n", stack_det);
    printf("Stack norm: %.2f\n", stack_norm);
    
    destroy_matrix(heap_matrix);
    return 0;
}