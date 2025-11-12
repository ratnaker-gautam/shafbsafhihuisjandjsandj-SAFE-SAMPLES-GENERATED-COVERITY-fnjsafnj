//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void initialize_matrix(Matrix *mat, int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = (double)((i + j) % 10);
        }
    }
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void matrix_transpose(const Matrix *src, Matrix *dest) {
    if (!validate_matrix_size(src->cols, src->rows)) {
        fprintf(stderr, "Invalid transpose dimensions\n");
        exit(1);
    }
    
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

double matrix_trace(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        fprintf(stderr, "Matrix must be square for trace\n");
        exit(1);
    }
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

int main() {
    Matrix a, b, result, transposed;
    
    printf("Matrix Operations Demo\n");
    printf("=====================\n\n");
    
    printf("Initializing matrices...\n");
    initialize_matrix(&a, 3, 4);
    initialize_matrix(&b, 4, 3);
    
    printf("\nMatrix A (3x4):\n");
    print_matrix(&a);
    
    printf("\nMatrix B (4x3):\n");
    print_matrix(&b);
    
    printf("\nPerforming matrix multiplication...\n");
    if (matrix_multiply(&a, &b, &result)) {
        printf("Result (3x3):\n");
        print_matrix(&result);
        
        printf("\nMatrix trace of result: %.2f\n", matrix_trace(&result));
    } else {
        printf("Matrix multiplication failed: dimension mismatch\n");
    }
    
    printf("\nTransposing matrix A...\n");
    matrix_transpose(&a, &transposed);
    printf("Transposed (4x3):\n");
    print_matrix(&transposed);
    
    printf("\nPerforming intensive computation...\n");
    Matrix large1, large2, large_result;
    initialize_matrix(&large1, 50, 50);
    initialize_matrix(&large2, 50, 50);
    
    clock_t start = clock();
    for (int iter = 0; iter < 100; iter++) {
        matrix_multiply(&large1, &large2, &large_result);
    }
    clock_t end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Completed 100 iterations of 50x50 matrix multiplication\n");
    printf("CPU time: %.3f seconds\n", cpu_time);
    
    return 0;
}