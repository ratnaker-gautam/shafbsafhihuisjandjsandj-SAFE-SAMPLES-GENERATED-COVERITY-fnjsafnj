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

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
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

double calculate_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols || mat->rows > 3) {
        fprintf(stderr, "Determinant only supported for small square matrices\n");
        exit(1);
    }
    
    if (mat->rows == 1) {
        return mat->data[0][0];
    }
    
    if (mat->rows == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    if (mat->rows == 3) {
        return mat->data[0][0] * (mat->data[1][1] * mat->data[2][2] - mat->data[1][2] * mat->data[2][1]) -
               mat->data[0][1] * (mat->data[1][0] * mat->data[2][2] - mat->data[1][2] * mat->data[2][0]) +
               mat->data[0][2] * (mat->data[1][0] * mat->data[2][1] - mat->data[1][1] * mat->data[2][0]);
    }
    
    return 0.0;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Demo\n");
    printf("=====================\n\n");
    
    printf("Initializing matrices...\n");
    initialize_matrix(&mat1, 3, 3);
    initialize_matrix(&mat2, 3, 3);
    
    printf("Matrix A:\n");
    print_matrix(&mat1);
    printf("\nMatrix B:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Matrix multiplication failed: dimension mismatch\n");
    }
    
    printf("\nMatrix Transpose (A):\n");
    transpose_matrix(&mat1, &result);
    print_matrix(&result);
    
    printf("\nDeterminant of Matrix A: %.2f\n", calculate_determinant(&mat1));
    
    printf("\nPerforming intensive matrix operations...\n");
    
    Matrix large_mat1, large_mat2, large_result;
    initialize_matrix(&large_mat1, 10, 10);
    initialize_matrix(&large_mat2, 10, 10);
    
    clock_t start = clock();
    
    for (int iter = 0; iter < 1000; iter++) {
        multiply_matrices(&large_mat1, &large_mat2, &large_result);
        transpose_matrix(&large_result, &large_mat1);
    }
    
    clock