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
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int temp = matrix[i][j];
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
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(matrix, size);
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, size);
    
    int original_diag = sum_diagonal(matrix, size);
    printf("Diagonal sum: %d\n", original_diag);
    
    transpose_matrix(matrix, size);
    
    printf("\nTransposed matrix:\n");
    print_matrix(matrix, size);
    
    int transposed_diag = sum_diagonal(matrix, size);
    printf("Diagonal sum after transpose: %d\n", transposed_diag);
    
    printf("\nMatrix elements by row:\n");
    for (int i = 0; i < size; i++) {
        printf("Row %d: ", i);
        int j = 0;
        while (j < size) {
            printf("%d", matrix[i][j]);
            if (j < size - 1) printf(", ");
            j++;
        }
        printf("\n");
    }
    
    printf("\nMatrix elements by column:\n");
    int col = 0;
    do {
        printf("Col %d: ", col);
        int row = 0;
        while (row < size) {
            printf("%d", matrix[row][col]);
            if (row < size - 1) printf(", ");
            row++;
        }
        printf("\n");
        col++;
    } while (col < size);
    
    return 0;
}