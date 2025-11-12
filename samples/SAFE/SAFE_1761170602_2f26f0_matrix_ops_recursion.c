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

void transposeRecursive(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size || col >= size) {
        return;
    }
    dest[col][row] = src[row][col];
    if (col + 1 < size) {
        transposeRecursive(src, dest, size, row, col + 1);
    } else if (row + 1 < size) {
        transposeRecursive(src, dest, size, row + 1, 0);
    }
}

int readInt(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        break;
    }
    return value;
}

int main() {
    int size = readInt("Enter matrix size (1-10): ", 1, MAX_SIZE);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    memset(matrix, 0, sizeof(matrix));
    
    fillMatrixRecursive(matrix, size, 0, 0, 1);
    
    printf("Original matrix:\n");
    printMatrix(matrix, size);
    
    int transposed[MAX_SIZE][MAX_SIZE];
    memset(transposed, 0, sizeof(transposed));
    
    transposeRecursive(matrix, transposed, size, 0, 0);
    
    printf("Transposed matrix:\n");
    printMatrix(transposed, size);
    
    return 0;
}