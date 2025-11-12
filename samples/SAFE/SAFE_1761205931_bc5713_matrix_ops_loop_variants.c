//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int counter = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int temp;
    int i = 0;
    while (i < size) {
        int j = i + 1;
        while (j < size) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
            j++;
        }
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int size) {
    int i = 0;
    do {
        int j = 0;
        do {
            result[i][j] = 0;
            int k = 0;
            while (k < size) {
                result[i][j] += a[i][k] * b[k][j];
                k++;
            }
            j++;
        } while (j < size);
        i++;
    } while (i < size);
}

int main() {
    int size;
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 1 || size > MAX_SIZE) {
        printf("Size must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(matrix_a, size);
    fill_matrix(matrix_b, size);
    
    printf("Matrix A:\n");
    print_matrix(matrix_a, size);
    
    printf("Matrix B:\n");
    print_matrix(matrix_b, size);
    
    transpose_matrix(matrix_a, size);
    printf("Transposed A:\n");
    print_matrix(matrix_a, size);
    
    multiply_matrices(matrix_a, matrix_b, result, size);
    printf("Result of A' * B:\n");
    print_matrix(result, size);
    
    return 0;
}