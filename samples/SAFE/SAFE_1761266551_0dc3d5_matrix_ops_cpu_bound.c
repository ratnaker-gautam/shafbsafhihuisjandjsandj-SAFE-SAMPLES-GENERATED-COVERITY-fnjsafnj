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
    srand((unsigned int)time(NULL));
    
    int size1 = 3 + rand() % 4;
    int size2 = 3 + rand() % 4;
    int size3 = size2;
    
    Matrix mat1, mat2, mat3, result;
    
    printf("Matrix A (%dx%d):\n", size1, size2);
    initialize_matrix(&mat1, size1, size2);
    print_matrix(&mat1);
    
    printf("\nMatrix B (%dx%d):\n", size2, size3);
    initialize_matrix(&mat2, size2, size3);
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication A * B:\n");
    if (matrix_multiply(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Multiplication failed: dimension mismatch\n");
    }
    
    printf("\nTranspose of A:\n");
    transpose_matrix(&mat1, &mat3);
    print_matrix(&mat3);
    
    if (mat1.rows == mat1.cols) {
        printf("\nTrace of A: %.2f\n", matrix_trace(&mat1));
    }
    
    if (mat2.rows == mat2.cols) {
        printf("Trace of B: %.2f\n", matrix_trace(&mat2));
    }
    
    return 0;
}