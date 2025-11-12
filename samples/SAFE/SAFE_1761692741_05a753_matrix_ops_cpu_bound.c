//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

void fill_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

void multiply_matrices(int m, int n, int p, int A[m][n], int B[n][p], int C[m][p]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    int m, n, p;
    printf("Enter dimensions for matrix A (rows columns): ");
    if (scanf("%d %d", &m, &n) != 2 || m <= 0 || n <= 0 || m > MAX_SIZE || n > MAX_SIZE) {
        printf("Invalid dimensions for matrix A\n");
        return 1;
    }
    
    printf("Enter columns for matrix B: ");
    if (scanf("%d", &p) != 1 || p <= 0 || p > MAX_SIZE) {
        printf("Invalid dimension for matrix B\n");
        return 1;
    }
    
    int A[m][n];
    int B[n][p];
    int C[m][p];
    
    fill_matrix(m, n, A);
    fill_matrix(n, p, B);
    
    printf("\nMatrix A (%dx%d):\n", m, n);
    print_matrix(m, n, A);
    
    printf("\nMatrix B (%dx%d):\n", n, p);
    print_matrix(n, p, B);
    
    multiply_matrices(m, n, p, A, B, C);
    
    printf("\nResult matrix C (%dx%d):\n", m, p);
    print_matrix(m, p, C);
    
    return 0;
}