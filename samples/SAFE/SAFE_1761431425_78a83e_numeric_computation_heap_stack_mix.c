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
    
    struct Matrix *mat_a = create_matrix(size, size);
    struct Matrix *mat_b = create_matrix(size, size);
    struct Matrix *result = create_matrix(size, size);
    
    if (!mat_a || !mat_b || !result) {
        printf("Memory allocation failed\n");
        if (mat_a) destroy_matrix(mat_a);
        if (mat_b) destroy_matrix(mat_b);
        if (result) destroy_matrix(result);
        return 1;
    }
    
    if (!initialize_matrix(mat_a) || !initialize_matrix(mat_b)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat_a);
        destroy_matrix(mat_b);
        destroy_matrix(result);
        return 1;
    }
    
    printf("Matrix A:\n");
    print_matrix(mat_a);
    printf("\nMatrix B:\n");
    print_matrix(mat_b);
    
    if (multiply_matrices(mat_a, mat_b, result)) {
        printf("\nResult of A * B:\n");
        print_matrix(result);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    if (size == 2) {
        double det = compute_determinant_2x2(mat_a);
        printf("\nDeterminant of Matrix A: %.2f\n", det);
    }
    
    destroy_matrix(mat_a);
    destroy_matrix(mat_b);
    destroy_matrix(result);
    
    return 0;
}