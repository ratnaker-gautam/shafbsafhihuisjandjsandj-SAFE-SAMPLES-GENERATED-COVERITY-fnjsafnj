//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix_recursive(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col, int value) {
    if (row >= size || col >= size) return;
    
    matrix[row][col] = value;
    
    if (col + 1 < size) {
        fill_matrix_recursive(matrix, size, row, col + 1, value + 1);
    } else if (row + 1 < size) {
        fill_matrix_recursive(matrix, size, row + 1, 0, value + 1);
    }
}

void transpose_recursive(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size || col >= size) return;
    
    dest[col][row] = src[row][col];
    
    if (col + 1 < size) {
        transpose_recursive(src, dest, size, row, col + 1);
    } else if (row + 1 < size) {
        transpose_recursive(src, dest, size, row + 1, 0);
    }
}

int sum_diagonal_recursive(int matrix[MAX_SIZE][MAX_SIZE], int size, int pos) {
    if (pos >= size) return 0;
    return matrix[pos][pos] + sum_diagonal_recursive(matrix, size, pos + 1);
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
    memset(matrix, 0, sizeof(matrix));
    
    fill_matrix_recursive(matrix, size, 0, 0, 1);
    
    printf("Original matrix:\n");
    print_matrix(matrix, size);
    
    int transposed[MAX_SIZE][MAX_SIZE];
    memset(transposed, 0, sizeof(transposed));
    
    transpose_recursive(matrix, transposed, size, 0, 0);
    
    printf("Transposed matrix:\n");
    print_matrix(transposed, size);
    
    int diagonal_sum = sum_diagonal_recursive(matrix, size, 0);
    printf("Diagonal sum: %d\n", diagonal_sum);
    
    return 0;
}