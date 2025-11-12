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
            mat->data[i][j] = (double)(i * cols + j + 1);
        }
    }
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
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

void transpose_matrix(const Matrix *input, Matrix *result) {
    if (!validate_matrix_size(input->cols, input->rows)) {
        fprintf(stderr, "Invalid dimensions for transpose\n");
        exit(1);
    }
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
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
    Matrix A, B, C, D;
    
    printf("Matrix Operations Demo\n");
    printf("=====================\n\n");
    
    printf("Matrix A (3x3):\n");
    initialize_matrix(&A, 3, 3);
    print_matrix(&A);
    
    printf("\nMatrix B (3x2):\n");
    initialize_matrix(&B, 3, 2);
    print_matrix(&B);
    
    printf("\nMatrix A Transpose:\n");
    transpose_matrix(&A, &C);
    print_matrix(&C);
    
    printf("\nMatrix A * B:\n");
    if (matrix_multiply(&A, &B, &D)) {
        print_matrix(&D);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    printf("\nTrace of Matrix A: %.2f\n", matrix_trace(&A));
    
    printf("\nPerforming intensive matrix operations...\n");
    
    clock_t start = clock();
    
    Matrix large1, large2, large_result;
    initialize_matrix(&large1, 50, 50);
    initialize_matrix(&large2, 50, 50);
    
    for (int iter = 0; iter < 100; iter++) {
        if (!matrix_multiply(&large1, &large2, &large_result)) {
            fprintf(stderr, "Large matrix multiplication failed\n");
            return 1;
        }
        transpose_matrix(&large_result, &large1);
    }
    
    clock_t end = clock();
    double cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("CPU-intensive operations completed in %.2f seconds\n", cpu_time);
    
    return 0;
}