//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("Enter element [%d][%d]: ", i, j);
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

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int size, int index) {
    if (index >= size) {
        return 0;
    }
    if (index < 0 || index >= MAX_SIZE) {
        return 0;
    }
    return matrix[index][index] + recursive_trace(matrix, size, index + 1);
}

void recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int size, int i, int j) {
    if (i >= size) {
        return;
    }
    if (j >= size) {
        recursive_transpose(src, dst, size, i + 1, 0);
        return;
    }
    if (i < MAX_SIZE && j < MAX_SIZE) {
        dst[j][i] = src[i][j];
    }
    recursive_transpose(src, dst, size, i, j + 1);
}

int main() {
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    int size;
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size <= 0 || size > MAX_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    
    printf("Enter matrix elements:\n");
    read_matrix(matrix, size);
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, size);
    
    printf("\nMatrix trace: %d\n", recursive_trace(matrix, size, 0));
    
    memset(transposed, 0, sizeof(transposed));
    recursive_transpose(matrix, transposed, size, 0, 0);
    
    printf("\nTransposed matrix:\n");
    print_matrix(transposed, size);
    
    return 0;
}