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

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) {
        return NULL;
    }
    
    struct Matrix *mat = malloc(sizeof(struct Matrix));
    if (mat == NULL) {
        return NULL;
    }
    
    mat->data = malloc(rows * cols * sizeof(double));
    if (mat->data == NULL) {
        free(mat);
        return NULL;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    return mat;
}

void destroy_matrix(struct Matrix *mat) {
    if (mat != NULL) {
        free(mat->data);
        free(mat);
    }
}

int initialize_matrix(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i * mat->cols + j] = (i + 1) * 0.5 + (j + 1) * 0.3;
        }
    }
    return 1;
}

double compute_determinant_2x2(struct Matrix *mat) {
    if (mat == NULL || mat->rows != 2 || mat->cols != 2) {
        return 0.0;
    }
    
    return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
}

double compute_matrix_norm(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int main() {
    int rows, cols;
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *heap_matrix = create_matrix(rows, cols);
    if (heap_matrix == NULL) {
        printf("Failed to create matrix\n");
        return 1;
    }
    
    if (!initialize_matrix(heap_matrix)) {
        printf("Failed to initialize matrix\n");
        destroy_matrix(heap_matrix);
        return 1;
    }
    
    printf("Matrix created and initialized successfully\n");
    
    double norm = compute_matrix_norm(heap_matrix);
    printf("Frobenius norm of matrix: %.6f\n", norm);
    
    if (rows == 2 && cols == 2) {
        double det = compute_determinant_2x2(heap_matrix);
        printf("Determinant of 2x2 matrix: %.6f\n", det);
    }
    
    struct Matrix stack_matrix;
    if (rows * cols <= 16) {
        double small_data[16];
        stack_matrix.data = small_data;
        stack_matrix.rows = rows;
        stack_matrix.cols = cols;
        
        for (int i = 0; i < rows * cols; i++) {
            stack_matrix.data[i] = heap_matrix->data[i] * 0.5;
        }
        
        double small_norm = compute_matrix_norm(&stack_matrix);
        printf("Norm of scaled stack matrix: %.6f\n", small_norm);
    }
    
    destroy_matrix(heap_matrix);
    
    return 0;
}