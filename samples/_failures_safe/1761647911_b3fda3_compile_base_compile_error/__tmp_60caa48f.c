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

double compute_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols || mat->rows > 3) {
        return 0.0;
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
    int choice;
    
    printf("Matrix Operations Demo\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Determinant Calculation\n");
    printf("3. Matrix Transpose\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            initialize_matrix(&a, 3, 2);
            initialize_matrix(&b, 2, 3);
            printf("Matrix A (3x2):\n");
            print_matrix(&a);
            printf("Matrix B (2x3):\n");
            print_matrix(&b);
            
            if (multiply_matrices(&a, &b, &result)) {
                printf("Result (A * B):\n");
                print_matrix(&result);
            } else {
                printf("Matrix multiplication failed\n");
            }
            break;
            
        case 2:
            initialize_matrix(&a, 3, 3);
            printf("Matrix (3x3):\n");
            print_matrix(&a);
            printf("Determinant: %.2f\n", compute_determinant(&a));
            break;
            
        case 3:
            initialize_matrix(&a, 2, 3);
            printf("Original Matrix (2x3):\n");
            print_matrix(&a);
            transpose_matrix(&a, &result);
            printf