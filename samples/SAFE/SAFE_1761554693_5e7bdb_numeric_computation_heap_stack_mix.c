//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

#define MAX_SIZE 100

struct Matrix {
    double *data;
    int rows;
    int cols;
};

int validate_matrix_params(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    if (cols > INT_MAX / rows) return 0;
    if (rows > INT_MAX / cols) return 0;
    return 1;
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_params(rows, cols)) return NULL;
    
    struct Matrix *mat = malloc(sizeof(struct Matrix));
    if (!mat) return NULL;
    
    mat->rows = rows;
    mat->cols = cols;
    size_t total_elements = (size_t)rows * (size_t)cols;
    if (total_elements > SIZE_MAX / sizeof(double)) {
        free(mat);
        return NULL;
    }
    mat->data = malloc(total_elements * sizeof(double));
    
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
    if (mat->rows == 2) {
        return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
    }
    
    double det = 0.0;
    for (int col = 0; col < mat->cols; col++) {
        struct Matrix *submat = create_matrix(mat->rows - 1, mat->cols - 1);
        if (!submat) return 0.0;
        
        int sub_i = 0;
        for (int i = 1; i < mat->rows; i++) {
            int sub_j = 0;
            for (int j = 0; j < mat->cols; j++) {
                if (j == col) continue;
                if (sub_i >= submat->rows || sub_j >= submat->cols) {
                    destroy_matrix(submat);
                    return 0.0;
                }
                submat->data[sub_i * submat->cols + sub_j] = mat->data[i * mat->cols + j];
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
    printf("Enter matrix size (1-3): ");
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 1 || size > 3) {
        printf("Size must be between 1 and 3\n");
        return 1;
    }
    
    struct Matrix *mat = create_matrix(size, size);
    if (!mat) {
        printf("Failed to create matrix\n");
        return 1;
    }
    
    if (!initialize_matrix(mat)) {
        printf("Failed to initialize matrix\n");
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