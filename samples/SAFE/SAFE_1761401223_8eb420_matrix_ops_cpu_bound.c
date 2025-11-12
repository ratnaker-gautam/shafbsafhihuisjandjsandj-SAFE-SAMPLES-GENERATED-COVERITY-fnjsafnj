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

void initialize_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = (i * mat->cols + j) % 10;
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%3d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void transpose_matrix(const Matrix* input, Matrix* result) {
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    printf("Enter dimensions for matrix A:\n");
    a.rows = read_int("Rows (1-100): ", 1, MAX_DIM);
    a.cols = read_int("Columns (1-100): ", 1, MAX_DIM);
    
    printf("\nEnter dimensions for matrix B:\n");
    b.rows = read_int("Rows (1-100): ", 1, MAX_DIM);
    b.cols = read_int("Columns (1-100): ", 1, MAX_DIM);
    
    if (a.rows == -1 || a.cols == -1 || b.rows == -1 || b.cols == -1) {
        printf("Error reading input.\n");
        return 1;
    }
    
    initialize_matrix(&a);
    initialize_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nPerforming matrix operations...\n\n");
    
    if (a.cols == b.rows) {
        result.rows = a.rows;
        result.cols = b.cols;
        multiply_matrices(&a, &b, &result);
        printf("Matrix A * B:\n");
        print_matrix(&result);
    } else {
        printf("Cannot multiply: incompatible dimensions (%dx%d * %dx%d)\n", 
               a.rows, a.cols, b.rows, b.cols);
    }
    
    printf("\nTranspose of A:\n");
    Matrix trans_a;
    trans_a.rows = a.cols;
    trans_a.cols = a.rows;
    transpose_matrix(&a, &trans_a);
    print_matrix(&trans_a);
    
    printf("\nTranspose of B:\n");
    Matrix trans_b;
    trans_b.rows = b.cols;
    trans_b.cols = b.rows;
    transpose_matrix(&b, &trans_b);
    print_matrix(&trans_b);
    
    return 0;
}