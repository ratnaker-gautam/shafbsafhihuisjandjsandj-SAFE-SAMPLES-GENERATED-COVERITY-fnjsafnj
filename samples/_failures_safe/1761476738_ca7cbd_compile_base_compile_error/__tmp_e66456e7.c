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

void initialize_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->data[i][j] = (double)((i + j) % 10);
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
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

double compute_determinant(const Matrix *m) {
    if (m->rows != m->cols || m->rows > 3) {
        return 0.0;
    }
    
    if (m->rows == 1) {
        return m->data[0][0];
    }
    
    if (m->rows == 2) {
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    }
    
    if (m->rows == 3) {
        return m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
               m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
               m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    }
    
    return 0.0;
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations\n");
    printf("1. Matrix multiplication\n");
    printf("2. Determinant calculation\n");
    printf("Enter choice (1-2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice == 1) {
        printf("Enter dimensions for matrix A (rows cols): ");
        if (scanf("%d %d", &a.rows, &a.cols) != 2) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        if (!validate_matrix_size(a.rows, a.cols)) {
            printf("Invalid matrix size\n");
            return 1;
        }
        
        printf("Enter dimensions for matrix B (rows cols): ");
        if (scanf("%d %d", &b.rows, &b.cols) != 2) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        if (!validate_matrix_size(b.rows, b.cols)) {
            printf("Invalid matrix size\n");
            return 1;
        }
        
        initialize_matrix(&a);
        initialize_matrix(&b);
        
        printf("\nMatrix A:\n");
        print_matrix(&a);
        printf("\nMatrix B:\n");
        print_matrix(&b);
        
        if (multiply_matrices(&a, &b, &result)) {
            printf("\nResult of A * B:\n");
            print_matrix(&result);
        } else {
            printf("Matrix multiplication failed: incompatible dimensions\n");
        }
    }
    else if (choice == 2) {
        printf("Enter square matrix size (1-3): ");
        if (scanf("%d", &a.rows) != 1) {
            printf("Invalid size\n");
            return 1;
        }
        
        a.cols = a.rows;
        
        if (!validate_matrix_size(a.rows, a.cols) || a.rows > 3) {
            printf("Invalid matrix size for determinant\n");
            return 1;
        }
        
        initialize_matrix(&a);
        printf("\nMatrix:\n");
        print_matrix(&a);
        
        double det = compute_determinant(&a);
        printf("Determinant: %.2f\n", det);