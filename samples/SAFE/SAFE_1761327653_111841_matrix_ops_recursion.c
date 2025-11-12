//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

int read_positive_int(const char* prompt, int max_val) {
    int value;
    char buffer[32];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value <= 0 || value > max_val) {
            printf("Value must be between 1 and %d.\n", max_val);
            continue;
        }
        
        return value;
    }
}

void recursive_matrix_multiply(int** A, int** B, int** C, int size, int row, int col, int k) {
    if (k >= size) {
        return;
    }
    
    C[row][col] += A[row][k] * B[k][col];
    recursive_matrix_multiply(A, B, C, size, row, col, k + 1);
}

void recursive_multiply_helper(int** A, int** B, int** C, int size, int row, int col) {
    if (col >= size) {
        return;
    }
    
    C[row][col] = 0;
    recursive_matrix_multiply(A, B, C, size, row, col, 0);
    recursive_multiply_helper(A, B, C, size, row, col + 1);
}

void recursive_matrix_fill(int** matrix, int size, int row, int col) {
    if (row >= size) {
        return;
    }
    
    if (col >= size) {
        recursive_matrix_fill(matrix, size, row + 1, 0);
        return;
    }
    
    char buffer[32];
    int value;
    
    while (1) {
        printf("Enter element [%d][%d]: ", row, col);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            matrix[row][col] = 0;
            return;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            matrix[row][col] = value;
            break;
        }
        printf("Invalid input. Please enter a number.\n");
    }
    
    recursive_matrix_fill(matrix, size, row, col + 1);
}

void recursive_matrix_print(int** matrix, int size, int row, int col) {
    if (row >= size) {
        return;
    }
    
    if (col >= size) {
        printf("\n");
        recursive_matrix_print(matrix, size, row + 1, 0);
        return;
    }
    
    printf("%6d", matrix[row][col]);
    recursive_matrix_print(matrix, size, row, col + 1);
}

void free_matrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int** allocate_matrix(int size) {
    int** matrix = (int**)malloc(size * sizeof(int*));
    if (matrix == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < size; i++) {
        matrix[i] = (int*)malloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    
    return matrix;
}

int main() {
    int size = read_positive_int("Enter matrix size (1-8): ", MAX_SIZE);
    if (size == -1) {
        return 1;
    }
    
    int** A = allocate_matrix(size);
    int** B = allocate_matrix(size);
    int** C = allocate_matrix(size);
    
    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed.\n");
        if (A) free_matrix(A, size);
        if (B) free_matrix(B, size);
        if (C) free_matrix(C, size);
        return 1;
    }
    
    printf("Enter elements for matrix A:\n");
    recursive_matrix_fill(A, size, 0, 0);
    
    printf("Enter elements for matrix B:\n");
    recursive_matrix_fill(B, size, 0, 0);
    
    for (int i = 0; i < size; i++) {
        recursive_multiply_helper(A, B, C, size, i, 0);
    }
    
    printf("\nMatrix A:\n");
    recursive_matrix_print(A, size, 0, 0);
    
    printf("\nMatrix B:\n");
    recursive_matrix_print(B, size, 0, 0);
    
    printf("\nResult (A * B):\n");
    recursive_matrix_print(C, size, 0, 0);
    
    free_matrix(A, size);
    free_matrix(B, size);
    free_matrix(C, size);
    
    return 0;
}