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
    double stack_values[4] = {1.5, 2.3, 3.7, 4.1};
    
    struct Matrix *mat = create_matrix(2, 2);
    if (!mat) {
        printf("Error: Failed to create matrix\n");
        return 1;
    }
    
    if (!initialize_matrix(mat, stack_values)) {
        printf("Error: Failed to initialize matrix\n");
        destroy_matrix(mat);
        return 1;
    }
    
    double determinant = compute_determinant_2x2(mat);
    double norm = compute_matrix_norm(mat);
    
    printf("Matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%.2f ", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
    
    printf("Determinant: %.4f\n", determinant);
    printf("Frobenius norm: %.4f\n", norm);
    
    double trace = mat->data[0] + mat->data[3];
    printf("Trace: %.4f\n", trace);
    
    destroy_matrix(mat);
    return 0;
}