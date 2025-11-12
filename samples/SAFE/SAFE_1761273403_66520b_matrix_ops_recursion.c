//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    if (scanf("%d %d", rows, cols) != 2) return 0;
    if (*rows <= 0 || *cols <= 0 || *rows > MAX_SIZE || *cols > MAX_SIZE) return 0;
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int i, int j, int rows, int cols) {
    if (i >= rows) return 1;
    if (j >= cols) return recursive_transpose(src, dst, i + 1, 0, rows, cols);
    dst[j][i] = src[i][j];
    return recursive_transpose(src, dst, i, j + 1, rows, cols);
}

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int n, int i) {
    if (i >= n) return 0;
    return matrix[i][i] + recursive_trace(matrix, n, i + 1);
}

int main() {
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols): ");
    if (!read_matrix(matrix, &rows, &cols)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original matrix:\n");
    print_matrix(matrix, rows, cols);
    
    if (rows == cols) {
        int trace = recursive_trace(matrix, rows, 0);
        printf("Matrix trace: %d\n", trace);
    }
    
    printf("Transposed matrix:\n");
    if (!recursive_transpose(matrix, transposed, 0, 0, rows, cols)) {
        printf("Transpose failed\n");
        return 1;
    }
    print_matrix(transposed, cols, rows);
    
    return 0;
}