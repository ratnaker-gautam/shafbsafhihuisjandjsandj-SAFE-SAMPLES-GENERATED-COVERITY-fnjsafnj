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
            double value = sin((double)(i + j) * 0.1) * 10.0;
            mat->data[i * mat->cols + j] = value;
        }
    }
    return 1;
}

double compute_matrix_norm(const struct Matrix *mat) {
    if (mat == NULL || mat->data == NULL) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
}

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    if (a->cols != b->rows || result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    
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

int main() {
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    struct Matrix *mat1 = create_matrix(rows, cols);
    struct Matrix *mat2 = create_matrix(cols, rows);
    struct Matrix *result = create_matrix(rows, rows);
    
    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        printf("Memory allocation failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        destroy_matrix(result);
        return 1;
    }
    
    if (!initialize_matrix(mat1) || !initialize_matrix(mat2)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(mat1);
        destroy_matrix(mat2);
        destroy_matrix(result);
        return 1;
    }
    
    double norm1 = compute_matrix_norm(mat1);
    double norm2 = compute_matrix_norm(mat2);
    printf("Matrix 1 norm: %.6f\n", norm1);
    printf("Matrix 2 norm: %.6f\n", norm2);
    
    if (multiply_matrices(mat1, mat2, result)) {
        double result_norm = compute_matrix_norm(result);
        printf("Result matrix norm: %.6f\n", result_norm);
        
        double stack_buffer[10];
        for (int i = 0; i < 10 && i < result->rows * result->cols; i++) {
            stack_buffer[i] = result->data[i];
        }
        
        printf("First 10 elements: ");
        for (int i = 0; i < 10 && i < result->rows * result->cols; i++) {
            printf("%.3f ", stack_buffer[i]);
        }
        printf("\n");
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(result);
    
    return 0;
}