//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void printMatrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void fillMatrixRecursive(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col, int value) {
    if (row >= size || col >= size) return;
    
    matrix[row][col] = value;
    
    if (col + 1 < size) {
        fillMatrixRecursive(matrix, size, row, col + 1, value + 1);
    } else if (row + 1 < size) {
        fillMatrixRecursive(matrix, size, row + 1, 0, value + 1);
    }
}

void transposeRecursive(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size || col >= size) return;
    
    if (row < col) {
        int temp = matrix[row][col];
        matrix[row][col] = matrix[col][row];
        matrix[col][row] = temp;
    }
    
    if (col + 1 < size) {
        transposeRecursive(matrix, size, row, col + 1);
    } else if (row + 1 < size) {
        transposeRecursive(matrix, size, row + 1, row + 2);
    }
}

int sumDiagonalRecursive(int matrix[MAX_SIZE][MAX_SIZE], int size, int pos) {
    if (pos >= size) return 0;
    return matrix[pos][pos] + sumDiagonalRecursive(matrix, size, pos + 1);
}

int main() {
    int size;
    char input[100];
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 1 || size > MAX_SIZE) {
        printf("Size must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    memset(matrix, 0, sizeof(matrix));
    
    fillMatrixRecursive(matrix, size, 0, 0, 1);
    
    printf("\nOriginal matrix:\n");
    printMatrix(matrix, size);
    
    printf("\nSum of main diagonal: %d\n", sumDiagonalRecursive(matrix, size, 0));
    
    transposeRecursive(matrix, size, 0, 1);
    
    printf("\nTransposed matrix:\n");
    printMatrix(matrix, size);
    
    printf("\nSum of main diagonal after transpose: %d\n", sumDiagonalRecursive(matrix, size, 0));
    
    return 0;
}