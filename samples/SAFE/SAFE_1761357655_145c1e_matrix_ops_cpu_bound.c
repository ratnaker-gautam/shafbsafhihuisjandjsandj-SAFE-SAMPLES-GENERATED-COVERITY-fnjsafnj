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
        double det = mat->data[0][0] * (mat->data[1][1] * mat->data[2][2] - mat->data[1][2] * mat->data[2][1])
                   - mat->data[0][1] * (mat->data[1][0] * mat->data[2][2] - mat->data[1][2] * mat->data[2][0])
                   + mat->data[0][2] * (mat->data[1][0] * mat->data[2][1] - mat->data[1][1] * mat->data[2][0]);
        return det;
    }
    
    return 0.0;
}

void transpose_matrix(const Matrix *mat, Matrix *result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int size;
    
    printf("Enter matrix size (1-10): ");
    if (scanf("%d", &size) != 1 || size < 1 || size > 10) {
        printf("Invalid size\n");
        return 1;
    }
    
    if (!validate_matrix_size(size, size)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    initialize_matrix(&mat1, size, size);
    initialize_matrix(&mat2, size, size);
    
    printf("\nMatrix A:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix B:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Multiplication failed\n");
    }
    
    printf("\nDeterminant of A: %.2f\n", compute_determinant(&mat1));
    printf("Determinant of B: %.2f\n", compute_determinant(&mat2));
    
    printf("\nTranspose of A:\n");
    transpose_matrix(&mat1, &result);
    print_matrix(&result);
    
    return 0;
}