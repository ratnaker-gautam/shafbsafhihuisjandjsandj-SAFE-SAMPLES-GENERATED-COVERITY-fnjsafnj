//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *mat) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) return 0;
    if (!validate_matrix_dimensions(mat->rows, mat->cols)) return 0;
    
    printf("Enter %d elements row by row:\n", mat->rows * mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

double compute_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) return 0.0;
    if (mat->rows == 1) return mat->data[0][0];
    if (mat->rows == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    double det = 0.0;
    for (int col = 0; col < mat->cols; col++) {
        Matrix submat;
        submat.rows = mat->rows - 1;
        submat.cols = mat->cols - 1;
        
        int sub_i = 0;
        for (int i = 1; i < mat->rows; i++) {
            int sub_j = 0;
            for (int j = 0; j < mat->cols; j++) {
                if (j == col) continue;
                submat.data[sub_i][sub_j] = mat->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * mat->data[0][col] * compute_determinant(&submat);
    }
    return det;
}

void transpose_matrix(const Matrix *mat, Matrix *result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = mat->data[j][i];
        }
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("========================\n");
    
    printf("Matrix A:\n");
    if (!read_matrix(&a)) {
        printf("Error: Invalid matrix A input\n");
        return 1;
    }
    
    printf("Matrix B:\n");
    if (!read_matrix(&b)) {
        printf("Error: Invalid matrix B input\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix Multiplication (A * B):\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply: dimension mismatch\n");
    }
    
    printf("\nDeterminant of A: ");
    if (a.rows == a.cols) {
        printf("%.2f\n", compute_determinant(&a));
    } else {
        printf("Not a square matrix\n");
    }
    
    printf("Determinant of B: ");
    if (b.rows == b.cols) {
        printf("%.2f\n", compute_determinant(&b));
    } else {
        printf("Not a square