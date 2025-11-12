//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int value = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = value++;
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int i = 0;
    while (i < size) {
        int j = 0;
        do {
            printf("%4d", matrix[i][j]);
            j++;
        } while (j < size);
        printf("\n");
        i++;
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int temp;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                      int result[MAX_SIZE][MAX_SIZE], int size) {
    int i = 0;
    while (i < size) {
        int j = 0;
        while (j < size) {
            result[i][j] = 0;
            int k = 0;
            do {
                result[i][j] += a[i][k] * b[k][j];
                k++;
            } while (k < size);
            j++;
        }
        i++;
    }
}

int get_valid_size(void) {
    char input[32];
    int size;
    
    while (1) {
        printf("Enter matrix size (1-%d): ", MAX_SIZE);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(input, "%d", &size) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (size < 1 || size > MAX_SIZE) {
            printf("Size must be between 1 and %d.\n", MAX_SIZE);
            continue;
        }
        
        return size;
    }
}

int main(void) {
    int size = get_valid_size();
    if (size == -1) {
        return 1;
    }
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(matrix_a, size);
    fill_matrix(matrix_b, size);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, size);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, size);
    
    transpose_matrix(matrix_a, size);
    printf("\nTransposed Matrix A:\n");
    print_matrix(matrix_a, size);
    
    multiply_matrices(matrix_a, matrix_b, result, size);
    printf("\nResult of A^T * B:\n");
    print_matrix(result, size);
    
    return 0;
}