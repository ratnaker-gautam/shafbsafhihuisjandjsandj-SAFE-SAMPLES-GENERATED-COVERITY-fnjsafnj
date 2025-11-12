//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
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

double matrix_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        fprintf(stderr, "Determinant requires square matrix\n");
        exit(1);
    }
    
    int n = mat->rows;
    if (n == 1) {
        return mat->data[0][0];
    }
    
    if (n == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    double det = 0.0;
    for (int col = 0; col < n; col++) {
        Matrix submat;
        submat.rows = n - 1;
        submat.cols = n - 1;
        
        for (int i = 1; i < n; i++) {
            int subcol = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                submat.data[i-1][subcol] = mat->data[i][j];
                subcol++;
            }
        }
        
        double subdet = matrix_determinant(&submat);
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * mat->data[0][col] * subdet;
    }
    
    return det;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    int size = 4;
    if (!validate_matrix_size(size, size)) {
        fprintf(stderr, "Invalid matrix size\n");
        return 1;
    }
    
    Matrix a, b, result;
    
    initialize_matrix(&a, size, size);
    initialize_matrix(&b, size, size);
    
    printf("Matrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    matrix_multiply(&a, &b, &result);
    printf("\nMatrix A * B:\n");
    print_matrix(&result);
    
    double det_a = matrix_determinant(&a);
    double det_b = matrix_determinant(&b);
    double det_result = matrix_determinant(&result);
    
    printf("\nDeterminants:\n");
    printf("det(A) = %.2f\n", det_a);
    printf("det(B) = %.2f\n", det_b);
    printf("det(A*B) = %.2f\n", det_result);
    printf("det(A)*det(B) = %.2f\n", det_a * det_b);
    
    return 0;
}