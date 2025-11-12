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
            double value = sin((double)(i + j) * 0.1) * cos((double)(i - j) * 0.1);
            mat->data[i * mat->cols + j] = value;
        }
    }
    return 1;
}

double compute_matrix_trace(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL || mat->rows != mat->cols) {
        return 0.0;
    }
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i * mat->cols + i];
    }
    return trace;
}

double compute_matrix_frobenius_norm(struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0.0;
    }
    
    double sum_squares = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum_squares += mat->data[i] * mat->data[i];
    }
    return sqrt(sum_squares);
}

int main() {
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *matrix = create_matrix(rows, cols);
    if (matrix == NULL) {
        printf("Failed to create matrix\n");
        return 1;
    }
    
    if (!initialize_matrix(matrix)) {
        printf("Failed to initialize matrix\n");
        destroy_matrix(matrix);
        return 1;
    }
    
    printf("Matrix created and initialized successfully\n");
    
    if (rows == cols) {
        double trace = compute_matrix_trace(matrix);
        printf("Matrix trace: %.6f\n", trace);
    } else {
        printf("Matrix is not square, skipping trace computation\n");
    }
    
    double norm = compute_matrix_frobenius_norm(matrix);
    printf("Frobenius norm: %.6f\n", norm);
    
    destroy_matrix(matrix);
    return 0;
}