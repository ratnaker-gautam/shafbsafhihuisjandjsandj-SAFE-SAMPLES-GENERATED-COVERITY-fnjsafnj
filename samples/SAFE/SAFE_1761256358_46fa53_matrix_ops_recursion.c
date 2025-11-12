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

void recursive_transpose(int matrix[MAX_SIZE][MAX_SIZE], int transposed[MAX_SIZE][MAX_SIZE], int row, int col, int size) {
    if (row >= size) return;
    if (col >= size) {
        recursive_transpose(matrix, transposed, row + 1, 0, size);
        return;
    }
    transposed[col][row] = matrix[row][col];
    recursive_transpose(matrix, transposed, row, col + 1, size);
}

int recursive_determinant(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    if (size == 1) return matrix[0][0];
    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    int det = 0;
    int sign = 1;
    int submatrix[MAX_SIZE][MAX_SIZE];
    
    for (int col = 0; col < size; col++) {
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
        det += sign * matrix[0][col] * recursive_determinant(submatrix, size - 1);
        sign = -sign;
    }
    return det;
}

int get_valid_size(void) {
    int size;
    char input[100];
    
    while (1) {
        printf("Enter matrix size (1-%d): ", MAX_SIZE);
        if (fgets(input, sizeof(input), stdin) == NULL) return -1;
        
        if (sscanf(input, "%d", &size) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (size < 1 || size > MAX_SIZE) {
            printf("Size must be between 1 and %d.\n", MAX_SIZE);
            continue;
        }
        
        return size;
    }
}

void get_matrix_input(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    char input[100];
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            while (1) {
                printf("Enter element [%d][%d]: ", i, j);
                if (fgets(input, sizeof(input), stdin) == NULL) return;
                
                if (sscanf(input, "%d", &matrix[i][j]) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    continue;
                }
                break;
            }
        }
    }
}

int main(void) {
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    int size;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    size = get_valid_size();
    if (size == -1) return 1;
    
    printf("\nEnter matrix elements:\n");
    get_matrix_input(matrix, size);
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, size);
    
    memset(transposed, 0, sizeof(transposed));
    recursive_transpose(matrix, transposed, 0, 0, size);
    
    printf("\nTransposed matrix:\n");
    print_matrix(transposed, size);
    
    int det = recursive_determinant(matrix, size);
    printf("\nDeterminant: %d\n", det);
    
    return 0;
}