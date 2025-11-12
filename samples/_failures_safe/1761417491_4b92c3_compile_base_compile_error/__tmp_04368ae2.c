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
    char input[256];
    
    printf("Enter number of rows: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &m->rows) != 1) return 0;
    if (!validate_dimension(m->rows)) return 0;
    
    printf("Enter number of columns: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &m->cols) != 1) return 0;
    if (!validate_dimension(m->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            if (sscanf(input, "%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) return 0;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MAX / b->data[k][j]) return 0;
                } else if (a->data[i][k] > 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] > INT_MIN / b->data[k][j]) return 0;
                } else if (a->data[i][k] < 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) return 0;
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *m, Matrix *result) {
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    int choice;
    char input[256];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf("Enter choice (1 or 2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice == 1) {
        printf("Enter first matrix:\n");
        if (!read_matrix(&a)) {
            printf("Error reading first matrix\n");
            return 1;
        }
        
        printf("Enter second matrix:\n");
        if (!read_matrix(&b)) {
            printf("Error reading second matrix\n");
            return 1;
        }
        
        if (!multiply_matrices(&a, &b, &result)) {
            printf("Matrix multiplication failed - dimension mismatch or overflow\n");
            return 1;
        }
        
        printf("Result of multiplication:\n");
        print_matrix(&result);
    } else {
        printf("Enter matrix:\n");
        if (!read_matrix(&a)) {
            printf("Error reading matrix\n");
            return 1;
        }
        
        if (!transpose_matrix(&a, &result))