//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MATRIX_SIZE 8

void fill_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

void multiply_matrices(int a[MATRIX_SIZE][MATRIX_SIZE], 
                      int b[MATRIX_SIZE][MATRIX_SIZE],
                      int result[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transpose_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], 
                     int result[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

void print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    int matrix_a[MATRIX_SIZE][MATRIX_SIZE];
    int matrix_b[MATRIX_SIZE][MATRIX_SIZE];
    int result_multiply[MATRIX_SIZE][MATRIX_SIZE];
    int result_transpose[MATRIX_SIZE][MATRIX_SIZE];
    
    fill_matrix(matrix_a);
    fill_matrix(matrix_b);
    
    printf("Matrix A:\n");
    print_matrix(matrix_a);
    printf("\nMatrix B:\n");
    print_matrix(matrix_b);
    
    multiply_matrices(matrix_a, matrix_b, result_multiply);
    printf("\nMatrix A * B:\n");
    print_matrix(result_multiply);
    
    transpose_matrix(matrix_a, result_transpose);
    printf("\nTranspose of A:\n");
    print_matrix(result_transpose);
    
    int trace = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        trace += matrix_a[i][i];
    }
    printf("\nTrace of A: %d\n", trace);
    
    int sum = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            sum += matrix_a[i][j];
        }
    }
    printf("Sum of all elements in A: %d\n", sum);
    
    return 0;
}