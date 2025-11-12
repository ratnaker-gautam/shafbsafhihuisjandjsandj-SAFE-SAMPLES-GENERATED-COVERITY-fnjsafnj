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

int validate_matrix_dim(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        result->data[i][j] = INT_MAX;
                        break;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        result->data[i][j] = INT_MIN;
                        break;
                    }
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void print_matrix(const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8d", m->data[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix_random(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->data[i][j] = rand() % 100 - 25;
        }
    }
}

int main(void) {
    Matrix a, b, result;
    
    printf("Matrix Multiplication Calculator\n");
    
    a.rows = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    if (a.rows == -1) return 1;
    
    a.cols = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    if (a.cols == -1) return 1;
    
    b.rows = a.cols;
    printf("Matrix B rows must match matrix A columns: %d\n", b.rows);
    
    b.cols = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    if (b.cols == -1) return 1;
    
    if (!validate_matrix_dim(a.rows) || !validate_matrix_dim(a.cols) ||
        !validate_matrix_dim(b.rows) || !validate_matrix_dim(b.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    
    printf("Filling matrices with random values...\n");
    fill_matrix_random(&a);
    fill_matrix_random(&b);
    
    printf("\nMatrix A (%dx%d):\n", a.rows, a.cols);
    print_matrix(&a);
    
    printf("\nMatrix B (%dx%d):\n", b.rows, b.cols);
    print_matrix(&b);
    
    printf("\nCalculating A × B...\n");
    matrix_multiply(&a, &b, &result);
    
    printf("\nResult Matrix (%dx%d):\n", result.rows, result.cols);
    print_matrix(&result);
    
    return 0;
}