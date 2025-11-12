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
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = 0.0;
        }
    }
}

int read_matrix(Matrix *mat) {
    int rows, cols;
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        return 0;
    }
    if (!validate_matrix_dimensions(rows, cols)) {
        return 0;
    }
    initialize_matrix(mat, rows, cols);
    printf("Enter %d x %d matrix elements:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
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

double matrix_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        return 0.0;
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
        initialize_matrix(&submat, n-1, n-1);
        for (int i = 1; i < n; i++) {
            int subcol = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                submat.data[i-1][subcol] = mat->data[i][j];
                subcol++;
            }
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * mat->data[0][col] * matrix_determinant(&submat);
    }
    return det;
}

void transpose_matrix(const Matrix *mat, Matrix *result) {
    initialize_matrix(result, mat->cols, mat->rows);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&a)) {
        printf("Error: Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Error: Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply: dimension mismatch\n");
    }
    
    printf("\nFirst matrix determinant: ");
    if (a.rows == a.cols) {
        printf("%.2f\n", matrix_determinant(&a));
    } else {
        printf("Not a square matrix\n");
    }
    
    printf("\nSecond matrix determinant: ");
    if (b.rows == b.cols) {
        printf("%.2f\n", matrix_determinant(&b));
    } else