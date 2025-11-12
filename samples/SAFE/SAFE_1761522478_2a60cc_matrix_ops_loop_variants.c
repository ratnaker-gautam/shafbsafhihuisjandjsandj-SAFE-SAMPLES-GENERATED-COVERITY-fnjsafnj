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
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < size) {
                if (INT_MAX - result[i][j] < a[i][k] * b[k][j]) {
                    fprintf(stderr, "Integer overflow detected\n");
                    exit(EXIT_FAILURE);
                }
                result[i][j] += a[i][k] * b[k][j];
                k++;
            }
        }
    }
}

int get_valid_size(void) {
    int size;
    char input[32];
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        exit(EXIT_FAILURE);
    }
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input: not a number\n");
        exit(EXIT_FAILURE);
    }
    
    if (val < 1 || val > MAX_SIZE) {
        fprintf(stderr, "Size must be between 1 and %d\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }
    
    size = (int)val;
    if (size != val) {
        fprintf(stderr, "Size out of integer range\n");
        exit(EXIT_FAILURE);
    }
    
    return size;
}

int main(void) {
    int size = get_valid_size();
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(matrix_a, size);
    fill_matrix(matrix_b, size);
    
    printf("Matrix A:\n");
    print_matrix(matrix_a, size);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, size);
    
    printf("\nTranspose of A:\n");
    transpose_matrix(matrix_a, size);
    print_matrix(matrix_a, size);
    transpose_matrix(matrix_a, size);
    
    printf("\nA * B:\n");
    multiply_matrices(matrix_a, matrix_b, result, size);
    print_matrix(result, size);
    
    return 0;
}