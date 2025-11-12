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

void print_matrix(struct Matrix *mat) {
    if (!mat || !mat->data) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
}

int main() {
    int size;
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 2 || size > 10) {
        printf("Size must be between 2 and 10\n");
        return 1;
    }
    
    struct Matrix *mat1 = create_matrix(size, size);
    struct Matrix *mat2 = create_matrix(size, size);
    struct Matrix *result = create_matrix(size, size);
    
    if (!mat1 || !mat2 || !result) {
        printf("Memory allocation failed\n");
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
    
    printf("Matrix 1:\n");
    print_matrix(mat1);
    printf("\nMatrix 2:\n");
    print_matrix(mat2);
    
    if (multiply_matrices(mat1, mat2, result)) {
        printf("\nMatrix multiplication result:\n");
        print_matrix(result);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    if (size == 2) {
        double det1 = compute_determinant_2x2(mat1);
        double det2 = compute_determinant_2x2(mat2);
        printf("\n2x2 Determinants: %.2f, %.2f\n", det1, det2);
    }
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(result);
    
    return 0;
}