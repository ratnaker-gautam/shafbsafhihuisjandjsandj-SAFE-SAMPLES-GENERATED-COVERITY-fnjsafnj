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

void recursiveTranspose(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size) return;
    
    if (col >= size) {
        recursiveTranspose(matrix, size, row + 1, 0);
        return;
    }
    
    if (row < col) {
        int temp = matrix[row][col];
        matrix[row][col] = matrix[col][row];
        matrix[col][row] = temp;
    }
    
    recursiveTranspose(matrix, size, row, col + 1);
}

int recursiveTrace(int matrix[MAX_SIZE][MAX_SIZE], int size, int index) {
    if (index >= size) return 0;
    return matrix[index][index] + recursiveTrace(matrix, size, index + 1);
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
    printf("Enter %d x %d matrix elements:\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nOriginal matrix:\n");
    printMatrix(matrix, size);
    
    printf("\nMatrix trace: %d\n", recursiveTrace(matrix, size, 0));
    
    int temp[MAX_SIZE][MAX_SIZE];
    memcpy(temp, matrix, sizeof(temp));
    recursiveTranspose(temp, size, 0, 0);
    
    printf("\nTransposed matrix:\n");
    printMatrix(temp, size);
    
    return 0;
}