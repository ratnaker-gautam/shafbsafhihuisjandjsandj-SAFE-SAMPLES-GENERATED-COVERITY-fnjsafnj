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

int sum_diagonal(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int sum = 0;
    int i = 0;
    while (i < size) {
        sum += matrix[i][i];
        i++;
    }
    return sum;
}

int main(void) {
    int size;
    char input[32];
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (val < 1 || val > MAX_SIZE) {
        fprintf(stderr, "Size out of range\n");
        return 1;
    }
    
    size = (int)val;
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(matrix, size);
    
    printf("Original matrix:\n");
    print_matrix(matrix, size);
    
    int diag_sum = sum_diagonal(matrix, size);
    printf("Diagonal sum: %d\n", diag_sum);
    
    transpose_matrix(matrix, size);
    
    printf("Transposed matrix:\n");
    print_matrix(matrix, size);
    
    diag_sum = sum_diagonal(matrix, size);
    printf("Diagonal sum after transpose: %d\n", diag_sum);
    
    return 0;
}