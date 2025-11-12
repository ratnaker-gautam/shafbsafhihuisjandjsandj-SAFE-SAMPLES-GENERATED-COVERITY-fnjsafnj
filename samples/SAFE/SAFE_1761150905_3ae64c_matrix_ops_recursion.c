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

void recursive_matrix_multiply(int** A, int** B, int** C, int size, int i, int j, int k) {
    if (i >= size) return;
    if (j >= size) {
        recursive_matrix_multiply(A, B, C, size, i + 1, 0, 0);
        return;
    }
    if (k >= size) {
        recursive_matrix_multiply(A, B, C, size, i, j + 1, 0);
        return;
    }
    
    if (k == 0) C[i][j] = 0;
    
    if (A[i][k] > 0 && B[k][j] > 0) {
        if (C[i][j] > INT_MAX - A[i][k] * B[k][j]) {
            C[i][j] = INT_MAX;
        } else {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
    
    recursive_matrix_multiply(A, B, C, size, i, j, k + 1);
}

void recursive_matrix_print(int** matrix, int size, int i, int j) {
    if (i >= size) {
        printf("\n");
        return;
    }
    if (j >= size) {
        printf("\n");
        recursive_matrix_print(matrix, size, i + 1, 0);
        return;
    }
    
    printf("%8d ", matrix[i][j]);
    recursive_matrix_print(matrix, size, i, j + 1);
}

void recursive_matrix_fill(int** matrix, int size, int i, int j) {
    if (i >= size) return;
    if (j >= size) {
        recursive_matrix_fill(matrix, size, i + 1, 0);
        return;
    }
    
    matrix[i][j] = rand() % 10 + 1;
    recursive_matrix_fill(matrix, size, i, j + 1);
}

int main(void) {
    int size;
    int** matrixA;
    int** matrixB;
    int** result;
    
    srand(42);
    
    size = read_positive_int("Enter matrix size (1-8): ", MAX_SIZE);
    if (size == -1) return 1;
    
    matrixA = malloc(size * sizeof(int*));
    matrixB = malloc(size * sizeof(int*));
    result = malloc(size * sizeof(int*));
    
    if (matrixA == NULL || matrixB == NULL || result == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    for (int i = 0; i < size; i++) {
        matrixA[i] = malloc(size * sizeof(int));
        matrixB[i] = malloc(size * sizeof(int));
        result[i] = malloc(size * sizeof(int));
        
        if (matrixA[i] == NULL || matrixB[i] == NULL || result[i] == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }
    }
    
    recursive_matrix_fill(matrixA, size, 0, 0);
    recursive_matrix_fill(matrixB, size, 0, 0);
    
    printf("\nMatrix A:\n");
    recursive_matrix_print(matrixA, size, 0, 0);
    
    printf("\nMatrix B:\n");
    recursive_matrix_print(matrixB, size, 0, 0);
    
    recursive_matrix_multiply(matrixA, matrixB, result, size, 0, 0, 0);
    
    printf("\nResult (A * B):\n");
    recursive_matrix_print(result, size, 0, 0);
    
    for (int i = 0; i < size; i++) {
        free(matrixA[i]);
        free(matrixB[i]);
        free(result[i]);
    }
    free(matrixA);
    free(matrixB);
    free(result);
    
    return 0;
}