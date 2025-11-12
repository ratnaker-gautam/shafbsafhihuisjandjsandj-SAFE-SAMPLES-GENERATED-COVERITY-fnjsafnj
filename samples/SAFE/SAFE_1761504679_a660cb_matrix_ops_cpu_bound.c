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
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = (double)((i + j) % 10);
        }
    }
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6.1f", mat->data[i][j]);
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

double compute_matrix_determinant(const Matrix *mat) {
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

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows1, cols1)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows2, cols2)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    Matrix mat1, mat2, result;
    
    initialize_matrix(&mat1, rows1, cols1);
    initialize_matrix(&mat2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    if (multiply_matrices(&mat1, &mat2, &result)) {
        printf("\nMatrix multiplication result:\n");
        print_matrix(&result);
        
        if (result.rows == result.cols && result.rows <= 3) {
            double det = compute_matrix_determinant(&result);
            printf("\nDeterminant of result: %.2f\n", det);
        }
    } else {
        printf("\nMatrix multiplication not possible\n");
    }
    
    if (mat1.rows == mat1.cols && mat1.rows <= 3) {
        double det1 = compute_matrix_determinant(&mat1);
        printf("Determinant of matrix 1: %.2f\n", det1);
    }
    
    if (mat2.rows == mat2.cols && mat2.rows <= 3) {
        double det2 = compute_matrix_determinant(&mat2);
        printf("Determinant of matrix 2: %.2f\n", det2);
    }
    
    return 0;
}