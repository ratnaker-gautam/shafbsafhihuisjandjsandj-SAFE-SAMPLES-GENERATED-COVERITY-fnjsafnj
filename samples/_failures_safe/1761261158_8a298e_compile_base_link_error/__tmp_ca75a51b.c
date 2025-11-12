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
            double value = sin((double)(i + j)) * cos((double)(i - j));
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
    
    double sum = 0.0;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        sum += mat->data[i] * mat->data[i];
    }
    return sqrt(sum);
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
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!initialize_matrix(matrix)) {
        printf("Matrix initialization failed\n");
        destroy_matrix(matrix);
        return 1;
    }
    
    double stack_buffer[10];
    for (int i = 0; i < 10; i++) {
        stack_buffer[i] = (double)i * 0.1;
    }
    
    double trace = 0.0;
    if (rows == cols) {
        trace = compute_matrix_trace(matrix);
        printf("Matrix trace: %.6f\n", trace);
    } else {
        printf("Matrix is not square, cannot compute trace\n");
    }
    
    double norm = compute_matrix_frobenius_norm(matrix);
    printf("Frobenius norm: %.6f\n", norm);
    
    double stack_sum = 0.0;
    for (int i = 0; i < 10; i++) {
        stack_sum += stack_buffer[i];
    }
    printf("Stack buffer sum: %.6f\n", stack_sum);
    
    double heap_stack_ratio = norm / (stack_sum + 1.0);
    printf("Heap/Stack ratio: %.6f\n", heap_stack_ratio);
    
    destroy_matrix(matrix);
    return 0;
}