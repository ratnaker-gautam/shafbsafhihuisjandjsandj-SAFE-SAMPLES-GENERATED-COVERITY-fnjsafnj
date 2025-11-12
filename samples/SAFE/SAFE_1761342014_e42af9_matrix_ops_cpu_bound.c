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

double matrix_trace(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        return 0.0;
    }
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

void transpose_matrix(const Matrix *mat, Matrix *result) {
    if (!validate_matrix_size(mat->cols, mat->rows)) {
        fprintf(stderr, "Invalid dimensions for transpose\n");
        exit(1);
    }
    result->rows = mat->cols;
    result->cols = mat->rows;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
}

int main() {
    Matrix a, b, result;
    int size = 4;
    
    if (!validate_matrix_size(size, size)) {
        fprintf(stderr, "Invalid matrix size\n");
        return 1;
    }
    
    initialize_matrix(&a, size, size);
    initialize_matrix(&b, size, size);
    
    printf("Matrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (matrix_multiply(&a, &b, &result)) {
        printf("\nMatrix A * B:\n");
        print_matrix(&result);
        printf("\nTrace of result: %.2f\n", matrix_trace(&result));
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    printf("\nTranspose of A:\n");
    transpose_matrix(&a, &result);
    print_matrix(&result);
    
    clock_t start = clock();
    for (int i = 0; i < 1000; i++) {
        matrix_multiply(&a, &b, &result);
    }
    clock_t end = clock();
    double cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU time for 1000 multiplications: %.4f seconds\n", cpu_time);
    
    return 0;
}