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

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *mat) {
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) {
        return 0;
    }
    if (!validate_dimension(mat->rows) || !validate_dimension(mat->cols)) {
        return 0;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%lf", &mat->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    printf("%d %d\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
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

int transpose_matrix(const Matrix *input, Matrix *result) {
    result->rows = input->cols;
    result->cols = input->rows;
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
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

int main(void) {
    Matrix a, b, result;
    int operation;
    
    printf("Enter operation (1=multiply, 2=transpose, 3=determinant): ");
    if (scanf("%d", &operation) != 1) {
        printf("Invalid operation\n");
        return 1;
    }
    
    switch (operation) {
        case 1:
            printf("Enter first matrix (rows cols data): ");
            if (!read_matrix(&a)) {
                printf("Invalid first matrix\n");
                return 1;
            }
            printf("Enter second matrix (rows cols data): ");
            if (!read_matrix(&b)) {
                printf("Invalid second matrix\n");
                return 1;
            }
            if (!multiply_matrices(&a, &b, &result)) {
                printf("Matrix dimensions incompatible for multiplication\n");
                return 1;
            }
            printf("Result:\n");
            print_matrix(&result);
            break;
            
        case 2:
            printf("Enter matrix (rows cols data): ");
            if (!read_matrix(&a)) {
                printf("Invalid matrix\n");
                return 1;
            }
            transpose_matrix(&a, &result);
            printf("Transpose:\n");
            print_matrix(&result);
            break;
            
        case 3:
            printf("Enter square matrix (rows cols data): ");
            if (!read_matrix(&a)) {
                printf("Invalid matrix\n");
                return 1;
            }
            if (a.rows != a.cols) {
                printf("Matrix must be square for determinant\n");
                return 1;
            }
            if (a.rows > 3) {
                printf("Determinant only supported for matrices up to