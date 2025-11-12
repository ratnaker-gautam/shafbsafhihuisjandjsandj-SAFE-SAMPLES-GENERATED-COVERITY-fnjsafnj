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

double compute_trace(struct Matrix *mat) {
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
    struct Matrix *mat2 = create_matrix(size, size);
    struct Matrix *result = create_matrix(size, size);
    
    if (!mat1 || !mat2 || !result) {
        printf("Matrix allocation failed\n");
        if (mat1) destroy_matrix(mat1);
        if (mat2) destroy_matrix(mat2);
        if (result) destroy_matrix(result);
        return 1;
    }
    
    if (!initialize_matrix(mat1) || !initialize_matrix(mat2)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        destroy_matrix(result);
        return 1;
    }
    
    double stack_matrix[4] = {2.0, 1.0, 1.0, 2.0};
    struct Matrix stack_mat = {stack_matrix, 2, 2};
    
    printf("Matrix operations:\n");
    
    double det = compute_determinant_2x2(&stack_mat);
    printf("2x2 determinant: %.2f\n", det);
    
    double trace1 = compute_trace(mat1);
    double trace2 = compute_trace(mat2);
    printf("Matrix 1 trace: %.2f\n", trace1);
    printf("Matrix 2 trace: %.2f\n", trace2);
    
    multiply_matrices(mat1, mat2, result);
    
    double result_trace = compute_trace(result);
    printf("Product matrix trace: %.2f\n", result_trace);
    
    double stack_vec[3] = {1.0, 2.0, 3.0};
    double dot_product = 0.0;
    for (int i = 0; i < 3; i++) {
        dot_product += stack_vec[i] * mat1->data[i * mat1->cols + i];
    }
    printf("Vector dot product with diagonal: %.2f\n", dot_product);
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(result);
    
    return 0;
}