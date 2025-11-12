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
    if (!validate_matrix_size(rows, cols)) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows || !validate_matrix_size(a->rows, b->cols)) {
        return;
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
    if (mat->rows != mat->cols || !validate_matrix_size(mat->rows, mat->cols)) {
        return 0.0;
    }
    
    int n = mat->rows;
    double temp[MAX_SIZE][MAX_SIZE];
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i][j] = mat->data[i][j];
        }
    }
    
    double det = 1.0;
    
    for (int i = 0; i < n; i++) {
        if (fabs(temp[i][i]) < 1e-12) {
            for (int j = i + 1; j < n; j++) {
                if (fabs(temp[j][i]) > 1e-12) {
                    for (int k = 0; k < n; k++) {
                        double swap = temp[i][k];
                        temp[i][k] = temp[j][k];
                        temp[j][k] = swap;
                    }
                    det = -det;
                    break;
                }
            }
        }
        
        if (fabs(temp[i][i]) < 1e-12) {
            return 0.0;
        }
        
        det *= temp[i][i];
        
        for (int j = i + 1; j < n; j++) {
            double factor = temp[j][i] / temp[i][i];
            for (int k = i + 1; k < n; k++) {
                temp[j][k] -= factor * temp[i][k];
            }
        }
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
    Matrix a, b, result;
    
    printf("Matrix Operations Demo\n");
    printf("Creating two 3x3 matrices...\n");
    
    initialize_matrix(&a, 3, 3);
    initialize_matrix(&b, 3, 3);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a.data[i][j] = i * 3 + j + 1;
            b.data[i][j] = (i == j) ? 2.0 : 1.0;
        }
    }
    
    printf("Matrix A:\n");
    print_matrix(&a);
    
    printf("Matrix B:\n");
    print_matrix(&b);
    
    printf("Multiplying A * B:\n");
    matrix_multiply(&a, &b, &result);
    print_matrix(&result);
    
    printf("Determinant of A: %.2f\n", matrix_determinant(&a));
    printf("Determinant of B: %.2f\n", matrix_determinant(&b));
    printf("Determinant of result: %.2f\n", matrix_determinant(&result));
    
    printf("Testing with invalid matrix...\n");
    Matrix invalid;
    initialize_matrix(&invalid, 150, 150);
    printf("Invalid matrix determinant: %.2f\n", matrix_determinant(&invalid));
    
    return 0;
}