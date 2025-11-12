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
        mat->data[i] = (double)(i % 10) + 1.0;
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (!mat || mat->rows != 2 || mat->cols != 2) return 0.0;
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_trace(struct Matrix *mat) {
    if (!mat || mat->rows != mat->cols) return 0.0;
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i * mat->cols + i];
    }
    return trace;
}

void multiply_matrices(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
    if (!a || !b || !result) return;
    if (a->cols != b->rows) return;
    if (result->rows != a->rows || result->cols != b->cols) return;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
}

int main() {
    int size = 3;
    
    struct Matrix *mat1 = create_matrix(size, size);
    if (!mat1) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    
    struct Matrix *mat2 = create_matrix(size, size);
    if (!mat2) {
        printf("Failed to create matrix 2\n");
        destroy_matrix(mat1);
        return 1;
    }
    
    struct Matrix *result = create_matrix(size, size);
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
    
    double trace1 = compute_matrix_trace(mat1);
    double trace2 = compute_matrix_trace(mat2);
    
    printf("Matrix 1 trace: %.2f\n", trace1);
    printf("Matrix 2 trace: %.2f\n", trace2);
    
    multiply_matrices(mat1, mat2, result);
    
    double result_trace = compute_matrix_trace(result);
    printf("Product matrix trace: %.2f\n", result_trace);
    
    struct Matrix small_mat_stack;
    double small_data[4] = {1.0, 2.0, 3.0, 4.0};
    small_mat_stack.data = small_data;
    small_mat_stack.rows = 2;
    small_mat_stack.cols = 2;
    
    double det = compute_determinant_2x2(&small_mat_stack);
    printf("2x2 determinant: %.2f\n", det);
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(result);
    
    return 0;
}