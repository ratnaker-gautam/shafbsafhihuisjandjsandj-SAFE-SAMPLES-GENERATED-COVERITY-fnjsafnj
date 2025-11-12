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
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_transpose(const Matrix* src, Matrix* dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

int matrix_trace(const Matrix* m) {
    int trace = 0;
    for (int i = 0; i < m->rows && i < m->cols; i++) {
        trace += m->data[i][i];
    }
    return trace;
}

void fill_matrix_random(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->data[i][j] = rand() % 100;
        }
    }
}

void print_matrix(const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand(42);
    
    printf("Matrix Operations Demo\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    
    if (rows1 == -1 || cols1 == -1) {
        return 1;
    }
    
    Matrix matA = {rows1, cols1};
    fill_matrix_random(&matA);
    
    printf("\nMatrix A:\n");
    print_matrix(&matA);
    
    int rows2 = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    
    if (rows2 == -1 || cols2 == -1) {
        return 1;
    }
    
    Matrix matB = {rows2, cols2};
    fill_matrix_random(&matB);
    
    printf("\nMatrix B:\n");
    print_matrix(&matB);
    
    if (matA.cols == matB.rows) {
        Matrix result = {matA.rows, matB.cols};
        matrix_multiply(&matA, &matB, &result);
        printf("\nMatrix A * B:\n");
        print_matrix(&result);
    } else {
        printf("\nCannot multiply: incompatible dimensions\n");
    }
    
    if (matA.rows == matA.cols) {
        printf("\nTrace of matrix A: %d\n", matrix_trace(&matA));
    }
    
    if (matB.rows == matB.cols) {
        printf("Trace of matrix B: %d\n", matrix_trace(&matB));
    }
    
    Matrix transA = {matA.cols, matA.rows};
    matrix_transpose(&matA, &transA);
    printf("\nTranspose of matrix A:\n");
    print_matrix(&transA);
    
    Matrix transB = {matB.cols, matB.rows};
    matrix_transpose(&matB, &transB);
    printf("\nTranspose of matrix B:\n");
    print_matrix(&transB);
    
    return 0;
}