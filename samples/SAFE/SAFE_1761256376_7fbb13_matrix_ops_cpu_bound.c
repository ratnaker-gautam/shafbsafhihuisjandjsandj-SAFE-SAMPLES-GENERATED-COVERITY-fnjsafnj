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
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *m) {
    char buffer[256];
    
    printf("Enter number of rows: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%d", &m->rows) != 1) return 0;
    if (!validate_dimension(m->rows)) return 0;
    
    printf("Enter number of columns: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%d", &m->cols) != 1) return 0;
    if (!validate_dimension(m->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
            if (sscanf(buffer, "%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
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
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
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

int main() {
    Matrix a, b, result;
    int choice;
    char buffer[256];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf("Enter choice: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    if (sscanf(buffer, "%d", &choice) != 1) return 1;
    
    switch (choice) {
        case 1:
            printf("Enter first matrix:\n");
            if (!read_matrix(&a)) {
                printf("Invalid input for first matrix\n");
                return 1;
            }
            printf("Enter second matrix:\n");
            if (!read_matrix(&b)) {
                printf("Invalid input for second matrix\n");
                return 1;
            }
            if (!multiply_matrices(&a, &b, &result)) {
                printf("Matrix multiplication failed - dimension mismatch or integer overflow\n");
                return 1;
            }
            printf("Result of multiplication:\n");
            print_matrix(&result);
            break;
            
        case 2:
            printf("Enter matrix to transpose:\n");
            if (!read_matrix(&a)) {
                printf("Invalid input for matrix\n");
                return 1;
            }
            if (!transpose_matrix(&a, &result)) {
                printf("Matrix transpose failed\n");
                return 1;
            }
            printf("Transposed matrix:\n");
            print_matrix(&result);
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}