//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    return (rows > 0 && rows <= MAX_DIM && cols > 0 && cols <= MAX_DIM);
}

void initialize_matrix(Matrix *mat, int rows, int cols) {
    if (!validate_matrix_dimensions(rows, cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = 0.0;
        }
    }
}

void read_matrix(Matrix *mat) {
    int rows, cols;
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_dimensions(rows, cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    
    initialize_matrix(mat, rows, cols);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
                exit(1);
            }
        }
    }
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int can_multiply(const Matrix *a, const Matrix *b) {
    return (a->cols == b->rows);
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!can_multiply(a, b)) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
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
}

double matrix_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        fprintf(stderr, "Determinant requires square matrix\n");
        exit(1);
    }
    
    if (mat->rows == 1) {
        return mat->data[0][0];
    }
    
    if (mat->rows == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    double det = 0.0;
    for (int col = 0; col < mat->cols; col++) {
        Matrix submat;
        initialize_matrix(&submat, mat->rows - 1, mat->cols - 1);
        
        for (int i = 1; i < mat->rows; i++) {
            int subcol = 0;
            for (int j = 0; j < mat->cols; j++) {
                if (j != col) {
                    submat.data[i-1][subcol] = mat->data[i][j];
                    subcol++;
                }
            }
        }
        
        double subdet = matrix_determinant(&submat);
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * mat->data[0][col] * subdet;
    }
    
    return det;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("\nFirst matrix:\n");
    read_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    read_matrix(&mat2);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication result:\n");
    matrix_multiply(&mat1, &mat2, &result);
    print_matrix(&result);
    
    if (mat1.rows == mat1.cols) {
        double det = matrix_determinant(&mat1);
        printf("\nDeterminant of first matrix: %.2f\n", det);
    }
    
    if (mat2.rows == mat2.cols) {
        double det = matrix_determinant(&mat2);
        printf("Determinant of second matrix: %.2f\n", det);
    }