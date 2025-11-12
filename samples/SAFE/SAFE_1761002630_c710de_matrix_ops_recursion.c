//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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

void recursive_transpose(int matrix[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size) {
        return;
    }
    if (col >= size) {
        recursive_transpose(matrix, size, row + 1, 0);
        return;
    }
    if (row < col) {
        int temp = matrix[row][col];
        matrix[row][col] = matrix[col][row];
        matrix[col][row] = temp;
    }
    recursive_transpose(matrix, size, row, col + 1);
}

int recursive_determinant(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    if (size == 1) {
        return matrix[0][0];
    }
    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    int det = 0;
    int sign = 1;
    for (int col = 0; col < size; col++) {
        int submatrix[MAX_SIZE][MAX_SIZE];
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                submatrix[sub_i][sub_j] = matrix[i][j];
                sub_j++;
            }
            sub_i++;
        }
        int sub_det = recursive_determinant(submatrix, size - 1);
        det += sign * matrix[0][col] * sub_det;
        sign = -sign;
    }
    return det;
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
    print_matrix(matrix, size);
    
    int original[MAX_SIZE][MAX_SIZE];
    memcpy(original, matrix, sizeof(original));
    
    recursive_transpose(matrix, size, 0, 0);
    printf("\nTransposed matrix:\n");
    print_matrix(matrix, size);
    
    int det = recursive_determinant(original, size);
    printf("\nDeterminant: %d\n", det);
    
    return 0;
}