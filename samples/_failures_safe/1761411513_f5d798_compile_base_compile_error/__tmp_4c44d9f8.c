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

int validate_matrix_dim(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *m) {
    char input[256];
    
    printf("Enter number of rows: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &m->rows) != 1) return 0;
    
    printf("Enter number of columns: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &m->cols) != 1) return 0;
    
    if (!validate_matrix_dim(m->rows, m->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            if (sscanf(input, "%lf", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
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

int transpose_matrix(const Matrix *m, Matrix *result) {
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = m->data[j][i];
        }
    }
    return 1;
}

double compute_determinant_2x2(const Matrix *m) {
    return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
}

int main() {
    Matrix a, b, result;
    int choice;
    char input[256];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf("3. 2x2 Determinant\n");
    printf("Enter choice: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    if (sscanf(input, "%d", &choice) != 1) return 1;
    
    switch (choice) {
        case 1:
            printf("Enter first matrix:\n");
            if (!read_matrix(&a)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            printf("Enter second matrix:\n");
            if (!read_matrix(&b)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            if (!multiply_matrices(&a, &b, &result)) {
                printf("Matrix dimensions incompatible for multiplication\n");
                return 1;
            }
            printf("Result of multiplication:\n");
            print_matrix(&result);
            break;
            
        case 2:
            printf("Enter matrix:\n");
            if (!read_matrix(&a)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            if (!transpose_matrix(&a, &result)) {
                printf("Transpose failed\n");
                return 1;
            }
            printf("Transpose:\n");
            print_matrix(&result);
            break;
            
        case 3:
            printf("Enter 2x2 matrix:\n");
            if (!read_matrix(&a)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            if (a.rows != 2 || a.cols != 2) {
                printf("Determinant only supported for 2x2 matrices\n");
                return 1;
            }
            printf("Determinant: %.2f\n", compute_determinant_2x2(&a));
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;