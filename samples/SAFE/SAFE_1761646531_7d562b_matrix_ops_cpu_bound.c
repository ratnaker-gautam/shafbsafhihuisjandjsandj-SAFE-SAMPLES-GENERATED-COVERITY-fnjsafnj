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
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

void init_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = rand() % 100;
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
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

void transpose_matrix(const Matrix* src, Matrix* dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    srand(42);
    
    printf("Matrix Operations Benchmark\n");
    printf("===========================\n\n");
    
    int rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    if (rows == -1) return 1;
    
    int cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    if (cols == -1) return 1;
    
    if (!validate_matrix_dim(rows) || !validate_matrix_dim(cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    Matrix mat_a, mat_b, mat_result;
    
    mat_a.rows = rows;
    mat_a.cols = cols;
    mat_b.rows = cols;
    mat_b.cols = rows;
    mat_result.rows = rows;
    mat_result.cols = rows;
    
    printf("\nInitializing matrices...\n");
    init_matrix(&mat_a);
    init_matrix(&mat_b);
    
    printf("\nMatrix A (%dx%d):\n", mat_a.rows, mat_a.cols);
    print_matrix(&mat_a);
    
    printf("\nMatrix B (%dx%d):\n", mat_b.rows, mat_b.cols);
    print_matrix(&mat_b);
    
    printf("\nPerforming matrix multiplication...\n");
    multiply_matrices(&mat_a, &mat_b, &mat_result);
    
    printf("\nResult of A * B (%dx%d):\n", mat_result.rows, mat_result.cols);
    print_matrix(&mat_result);
    
    printf("\nPerforming matrix transpose...\n");
    Matrix mat_transpose;
    mat_transpose.rows = mat_a.cols;
    mat_transpose.cols = mat_a.rows;
    transpose_matrix(&mat_a, &mat_transpose);
    
    printf("\nTranspose of A (%dx%d):\n", mat_transpose.rows, mat_transpose.cols);
    print_matrix(&mat_transpose);
    
    printf("\nBenchmark completed.\n");
    return 0;
}