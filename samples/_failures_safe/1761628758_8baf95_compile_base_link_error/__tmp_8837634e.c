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

double compute_matrix_norm(struct Matrix *mat) {
    if (!mat || !mat->data) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int main() {
    int rows = 2, cols = 2;
    
    struct Matrix *heap_matrix = create_matrix(rows, cols);
    if (!heap_matrix) {
        fprintf(stderr, "Failed to create heap matrix\n");
        return 1;
    }
    
    if (!initialize_matrix(heap_matrix)) {
        fprintf(stderr, "Failed to initialize heap matrix\n");
        destroy_matrix(heap_matrix);
        return 1;
    }
    
    double stack_vector[MAX_SIZE];
    int vec_size = rows * cols;
    
    if (vec_size > MAX_SIZE) {
        fprintf(stderr, "Vector size exceeds maximum\n");
        destroy_matrix(heap_matrix);
        return 1;
    }
    
    for (int i = 0; i < vec_size; i++) {
        stack_vector[i] = heap_matrix->data[i] * 2.0;
    }
    
    double determinant = compute_determinant_2x2(heap_matrix);
    double norm = compute_matrix_norm(heap_matrix);
    
    double stack_sum = 0.0;
    for (int i = 0; i < vec_size; i++) {
        stack_sum += stack_vector[i];
    }
    
    printf("Matrix determinant: %.2f\n", determinant);
    printf("Matrix norm: %.2f\n", norm);
    printf("Stack vector sum: %.2f\n", stack_sum);
    
    destroy_matrix(heap_matrix);
    return 0;
}