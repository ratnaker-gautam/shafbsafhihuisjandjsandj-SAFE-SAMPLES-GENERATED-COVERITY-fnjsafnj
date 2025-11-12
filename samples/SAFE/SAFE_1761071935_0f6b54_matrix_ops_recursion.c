//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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
    if (row >= size || col >= size) {
        return;
    }
    
    matrix[row][col] = value;
    
    if (col + 1 < size) {
        fillMatrixRecursive(matrix, size, row, col + 1, value + 1);
    } else if (row + 1 < size) {
        fillMatrixRecursive(matrix, size, row + 1, 0, value + 1);
    }
}

void transposeMatrixRecursive(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size || col >= size) {
        return;
    }
    
    dest[col][row] = src[row][col];
    
    if (col + 1 < size) {
        transposeMatrixRecursive(src, dest, size, row, col + 1);
    } else if (row + 1 < size) {
        transposeMatrixRecursive(src, dest, size, row + 1, 0);
    }
}

int getValidSize() {
    int size;
    char input[100];
    
    while (1) {
        printf("Enter matrix size (1-%d): ", MAX_SIZE);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input\n");
            exit(1);
        }
        
        if (sscanf(input, "%d", &size) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (size < 1 || size > MAX_SIZE) {
            printf("Size must be between 1 and %d\n", MAX_SIZE);
            continue;
        }
        
        break;
    }
    
    return size;
}

int main() {
    int size = getValidSize();
    
    int matrixA[MAX_SIZE][MAX_SIZE];
    int matrixB[MAX_SIZE][MAX_SIZE];
    
    memset(matrixA, 0, sizeof(matrixA));
    memset(matrixB, 0, sizeof(matrixB));
    
    printf("\nFilling matrix with sequential values:\n");
    fillMatrixRecursive(matrixA, size, 0, 0, 1);
    printMatrix(matrixA, size);
    
    printf("\nTransposing matrix:\n");
    transposeMatrixRecursive(matrixA, matrixB, size, 0, 0);
    printMatrix(matrixB, size);
    
    printf("\nVerifying transpose by transposing back:\n");
    int matrixC[MAX_SIZE][MAX_SIZE];
    memset(matrixC, 0, sizeof(matrixC));
    transposeMatrixRecursive(matrixB, matrixC, size, 0, 0);
    printMatrix(matrixC, size);
    
    int identical = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrixA[i][j] != matrixC[i][j]) {
                identical = 0;
                break;
            }
        }
        if (!identical) break;
    }
    
    if (identical) {
        printf("\nTranspose verification successful!\n");
    } else {
        printf("\nTranspose verification failed!\n");
    }
    
    return 0;
}