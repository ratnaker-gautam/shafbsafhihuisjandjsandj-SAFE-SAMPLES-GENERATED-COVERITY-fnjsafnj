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
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
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

int get_valid_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min, max);
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    a.rows = get_valid_int("Enter rows for matrix A (1-100): ", 1, MAX_SIZE);
    a.cols = get_valid_int("Enter columns for matrix A (1-100): ", 1, MAX_SIZE);
    initialize_matrix(&a);
    
    b.rows = get_valid_int("Enter rows for matrix B (1-100): ", 1, MAX_SIZE);
    b.cols = get_valid_int("Enter columns for matrix B (1-100): ", 1, MAX_SIZE);
    initialize_matrix(&b);
    
    printf("\nMatrix A (%dx%d):\n", a.rows, a.cols);
    print_matrix(&a);
    
    printf("\nMatrix B (%dx%d):\n", b.rows, b.cols);
    print_matrix(&b);
    
    printf("\nPerforming matrix multiplication...\n");
    if (multiply_matrices(&a, &b, &result)) {
        printf("Result matrix (%dx%d):\n", result.rows, result.cols);
        print_matrix(&result);
    } else {
        printf("Matrix multiplication failed: incompatible dimensions.\n");
    }
    
    printf("\nComputing determinants...\n");
    double det_a = compute_determinant(&a);
    double det_b = compute_determinant(&b);
    
    printf("Determinant of A: %.2f\n", det_a);
    printf("Determinant of B: %.2f\n", det_b);
    
    if (a.rows == a.cols && b.rows == b.cols) {
        printf("Product of determinants: %.2f\n", det_a * det_b);
    }
    
    return 0;
}