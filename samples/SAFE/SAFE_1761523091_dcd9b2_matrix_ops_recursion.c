//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_SIZE 10

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int recursive_determinant(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    if (size == 1) {
        return matrix[0][0];
    }
    
    int det = 0;
    int sign = 1;
    int submatrix[MAX_SIZE][MAX_SIZE];
    
    for (int col = 0; col < size; col++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j != col) {
                    submatrix[sub_i][sub_j] = matrix[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        int minor = recursive_determinant(submatrix, size - 1);
        if (minor > INT_MAX / size || minor < INT_MIN / size) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        
        int term = sign * matrix[0][col] * minor;
        if ((term > 0 && det > INT_MAX - term) || (term < 0 && det < INT_MIN - term)) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        
        det += term;
        sign = -sign;
    }
    
    return det;
}

void recursive_transpose(int matrix[MAX_SIZE][MAX_SIZE], int transposed[MAX_SIZE][MAX_SIZE], int row, int col, int size) {
    if (row >= size) {
        return;
    }
    
    if (col >= size) {
        recursive_transpose(matrix, transposed, row + 1, 0, size);
        return;
    }
    
    transposed[col][row] = matrix[row][col];
    recursive_transpose(matrix, transposed, row, col + 1, size);
}

int main() {
    int size;
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 1 || size > MAX_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    printf("Enter %dx%d matrix elements:\n", size, size);
    read_matrix(matrix, size);
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, size);
    
    int determinant = recursive_determinant(matrix, size);
    printf("\nDeterminant: %d\n", determinant);
    
    int transposed[MAX_SIZE][MAX_SIZE];
    recursive_transpose(matrix, transposed, 0, 0, size);
    
    printf("\nTransposed matrix:\n");
    print_matrix(transposed, size);
    
    return 0;
}