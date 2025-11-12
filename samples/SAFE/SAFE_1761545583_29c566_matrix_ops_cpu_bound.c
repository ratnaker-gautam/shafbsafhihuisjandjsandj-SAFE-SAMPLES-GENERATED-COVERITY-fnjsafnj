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
        printf("Invalid matrix dimensions\n");
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

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        printf("Result matrix would exceed maximum size\n");
        exit(1);
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
}

void matrix_transpose(const Matrix *input, Matrix *result) {
    if (!validate_matrix_size(input->cols, input->rows)) {
        printf("Transposed matrix would exceed maximum size\n");
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

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix mat1, mat2, result1, result2;
    
    printf("Matrix Operations Demo\n");
    printf("=====================\n\n");
    
    printf("Initializing matrices...\n");
    initialize_matrix(&mat1, 3, 4);
    initialize_matrix(&mat2, 4, 3);
    
    printf("\nMatrix A (3x4):\n");
    print_matrix(&mat1);
    
    printf("\nMatrix B (4x3):\n");
    print_matrix(&mat2);
    
    printf("\nComputing A × B...\n");
    matrix_multiply(&mat1, &mat2, &result1);
    printf("Result (3x3):\n");
    print_matrix(&result1);
    
    printf("\nComputing transpose of A...\n");
    matrix_transpose(&mat1, &result2);
    printf("Transpose of A (4x3):\n");
    print_matrix(&result2);
    
    printf("\nComputing transpose of B...\n");
    matrix_transpose(&mat2, &result1);
    printf("Transpose of B (3x4):\n");
    print_matrix(&result1);
    
    printf("\nComputing B × A...\n");
    matrix_multiply(&mat2, &mat1, &result2);
    printf("Result (4x4):\n");
    print_matrix(&result2);
    
    printf("\nComputing (A × B) × A...\n");
    matrix_multiply(&result1, &mat1, &result2);
    printf("Result (3x4):\n");
    print_matrix(&result2);
    
    return 0;
}