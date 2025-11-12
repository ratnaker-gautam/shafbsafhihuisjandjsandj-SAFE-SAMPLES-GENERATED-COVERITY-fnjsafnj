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
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input: not a number\n");
        return 1;
    }
    
    if (val < 1 || val > MAX_SIZE) {
        fprintf(stderr, "Size must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    size = (int)val;
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(matrix, size);
    
    printf("Original matrix:\n");
    print_matrix(matrix, size);
    
    transpose_matrix(matrix, size);
    
    printf("Transposed matrix:\n");
    print_matrix(matrix, size);
    
    int sum = 0;
    int count = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] % 2 == 0) {
                sum += matrix[i][j];
                count++;
            }
        }
    }
    
    printf("Sum of even elements: %d\n", sum);
    printf("Count of even elements: %d\n", count);
    
    int diag_sum = 0;
    int k = 0;
    while (k < size) {
        diag_sum += matrix[k][k];
        k++;
    }
    printf("Diagonal sum: %d\n", diag_sum);
    
    return 0;
}