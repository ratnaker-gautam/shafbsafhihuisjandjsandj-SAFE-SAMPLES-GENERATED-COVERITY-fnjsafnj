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

void transpose_recursive(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
    if (row >= size || col >= size) return;
    
    dst[col][row] = src[row][col];
    
    if (col + 1 < size) {
        transpose_recursive(src, dst, size, row, col + 1);
    } else if (row + 1 < size) {
        transpose_recursive(src, dst, size, row + 1, 0);
    }
}

int get_valid_size(void) {
    char input[32];
    int size;
    
    while (1) {
        printf("Enter matrix size (1-%d): ", MAX_SIZE);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            continue;
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

int main(void) {
    int size = get_valid_size();
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    memset(matrix, 0, sizeof(matrix));
    memset(transposed, 0, sizeof(transposed));
    
    printf("\nOriginal matrix:\n");
    fill_matrix_recursive(matrix, size, 0, 0, 1);
    print_matrix(matrix, size);
    
    printf("\nTransposed matrix:\n");
    transpose_recursive(matrix, transposed, size, 0, 0);
    print_matrix(transposed, size);
    
    return 0;
}