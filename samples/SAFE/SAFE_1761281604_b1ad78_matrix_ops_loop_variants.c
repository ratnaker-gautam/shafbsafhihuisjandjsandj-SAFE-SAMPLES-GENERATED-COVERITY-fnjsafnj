//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int counter = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = counter++;
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
            int sum = 0;
            int k = 0;
            do {
                sum += a[i][k] * b[k][j];
                k++;
            } while (k < size);
            result[i][j] = sum;
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
        
        char *endptr;
        long val = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (val < 1 || val > MAX_SIZE) {
            printf("Size must be between 1 and %d.\n", MAX_SIZE);
            continue;
        }
        
        size = (int)val;
        break;
    }
    
    return size;
}

int main(void) {
    int size = get_valid_size();
    if (size == -1) {
        return 1;
    }
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("\nMatrix A:\n");
    fill_matrix(matrix_a, size);
    print_matrix(matrix_a, size);
    
    printf("\nMatrix B:\n");
    fill_matrix(matrix_b, size);
    print_matrix(matrix_b, size);
    
    printf("\nTranspose of Matrix A:\n");
    transpose_matrix(matrix_a, size);
    print_matrix(matrix_a, size);
    
    printf("\nMatrix A * Matrix B:\n");
    multiply_matrices(matrix_a, matrix_b, result, size);
    print_matrix(result, size);
    
    return 0;
}