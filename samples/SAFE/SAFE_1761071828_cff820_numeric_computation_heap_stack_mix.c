//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
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

void print_matrix(const struct Matrix *mat) {
    if (!mat || !mat->data) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int main() {
    int rows1 = 3, cols1 = 4;
    int rows2 = 4, cols2 = 2;
    
    if (!validate_matrix_params(rows1, cols1) || !validate_matrix_params(rows2, cols2)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *mat1 = create_matrix(rows1, cols1);
    struct Matrix *mat2 = create_matrix(rows2, cols2);
    struct Matrix *result = create_matrix(rows1, cols2);
    
    if (!mat1 || !mat2 || !result) {
        fprintf(stderr, "Memory allocation failed\n");
        if (mat1) destroy_matrix(mat1);
        if (mat2) destroy_matrix(mat2);
        if (result) destroy_matrix(result);
        return 1;
    }
    
    if (!initialize_matrix(mat1) || !initialize_matrix(mat2)) {
        fprintf(stderr, "Matrix initialization failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        destroy_matrix(result);
        return 1;
    }
    
    printf("Matrix A (%dx%d):\n", rows1, cols1);
    print_matrix(mat1);
    printf("\nMatrix B (%dx%d):\n", rows2, cols2);
    print_matrix(mat2);
    
    if (multiply_matrices(mat1, mat2, result)) {
        printf("\nResult matrix (%dx%d):\n", rows1, cols2);
        print_matrix(result);
    } else {
        fprintf(stderr, "Matrix multiplication failed\n");
    }
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(result);
    
    return 0;
}