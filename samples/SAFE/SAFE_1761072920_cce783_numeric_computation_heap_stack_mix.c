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
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i * mat->cols + j] = (i + 1) * (j + 1) * 0.1;
        }
    }
    return 1;
}

double compute_matrix_determinant(struct Matrix *mat) {
    if (!mat || !mat->data) return 0.0;
    if (mat->rows != mat->cols) return 0.0;
    if (mat->rows == 1) return mat->data[0];
    
    double det = 0.0;
    int n = mat->rows;
    
    if (n == 2) {
        return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
    }
    
    for (int col = 0; col < n; col++) {
        struct Matrix *submat = create_matrix(n - 1, n - 1);
        if (!submat) return 0.0;
        
        int sub_i = 0;
        for (int i = 1; i < n; i++) {
            int sub_j = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                submat->data[sub_i * (n - 1) + sub_j] = mat->data[i * n + j];
                sub_j++;
            }
            sub_i++;
        }
        
        double sub_det = compute_matrix_determinant(submat);
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * mat->data[col] * sub_det;
        
        destroy_matrix(submat);
    }
    
    return det;
}

int main() {
    int size;
    
    printf("Enter matrix size (1-5): ");
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 1 || size > 5) {
        printf("Size must be between 1 and 5\n");
        return 1;
    }
    
    struct Matrix *mat = create_matrix(size, size);
    if (!mat) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!initialize_matrix(mat)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat);
        return 1;
    }
    
    printf("Matrix contents:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6.2f ", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
    
    double det = compute_matrix_determinant(mat);
    printf("Determinant: %.4f\n", det);
    
    destroy_matrix(mat);
    return 0;
}