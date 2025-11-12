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
                int idx_a = i * a->cols + k;
                int idx_b = k * b->cols + j;
                sum += a->data[idx_a] * b->data[idx_b];
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
    struct Matrix stack_mat;
    double stack_data[4] = {1.0, 2.0, 3.0, 4.0};
    stack_mat.data = stack_data;
    stack_mat.rows = 2;
    stack_mat.cols = 2;
    
    printf("Stack matrix:\n");
    print_matrix(&stack_mat);
    printf("Determinant: %.2f\n\n", compute_determinant_2x2(&stack_mat));
    
    struct Matrix *heap_mat1 = create_matrix(2, 3);
    struct Matrix *heap_mat2 = create_matrix(3, 2);
    struct Matrix *heap_result = create_matrix(2, 2);
    
    if (!heap_mat1 || !heap_mat2 || !heap_result) {
        printf("Matrix creation failed\n");
        destroy_matrix(heap_mat1);
        destroy_matrix(heap_mat2);
        destroy_matrix(heap_result);
        return 1;
    }
    
    initialize_matrix(heap_mat1);
    initialize_matrix(heap_mat2);
    
    printf("Heap matrix 1:\n");
    print_matrix(heap_mat1);
    printf("\nHeap matrix 2:\n");
    print_matrix(heap_mat2);
    
    if (multiply_matrices(heap_mat1, heap_mat2, heap_result)) {
        printf("\nMatrix product:\n");
        print_matrix(heap_result);
    } else {
        printf("\nMatrix multiplication failed\n");
    }
    
    destroy_matrix(heap_mat1);
    destroy_matrix(heap_mat2);
    destroy_matrix(heap_result);
    
    return 0;
}