//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

typedef struct {
    double data[MAX_SIZE][MAX_SIZE];
    int rows;
    int cols;
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void initialize_matrix(Matrix *mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = 0.0;
        }
    }
}

void fill_matrix_sequential(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = i * mat->cols + j + 1;
        }
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        return 0;
    }
    
    initialize_matrix(result, a->rows, b->cols);
    
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
    initialize_matrix(result, input->cols, input->rows);
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double calculate_frobenius_norm(const Matrix *mat) {
    double sum = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            sum += mat->data[i][j] * mat->data[i][j];
        }
    }
    return sqrt(sum);
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int size;
    printf("Enter matrix size (2-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(size, size)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    Matrix a, b, result;
    
    initialize_matrix(&a, size, size);
    initialize_matrix(&b, size, size);
    
    fill_matrix_sequential(&a);
    fill_matrix_sequential(&b);
    
    printf("Matrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (multiply_matrices(&a, &b, &result)) {
        printf("\nMatrix A * B:\n");
        print_matrix(&result);
        
        double norm = calculate_frobenius_norm(&result);
        printf("\nFrobenius norm of result: %.4f\n", norm);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    Matrix transposed;
    transpose_matrix(&a, &transposed);
    printf("\nTranspose of A:\n");
    print_matrix(&transposed);
    
    return 0;
}