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

int recursive_transpose(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int i, int j, int rows, int cols) {
    if (i >= rows) return 1;
    if (j >= cols) return recursive_transpose(src, dest, i + 1, 0, rows, cols);
    dest[j][i] = src[i][j];
    return recursive_transpose(src, dest, i, j + 1, rows, cols);
}

int recursive_trace(int matrix[MAX_SIZE][MAX_SIZE], int i, int size, int sum) {
    if (i >= size) return sum;
    return recursive_trace(matrix, i + 1, size, sum + matrix[i][i]);
}

int recursive_determinant(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    if (size == 1) return matrix[0][0];
    if (size == 2) return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    
    int det = 0;
    int sign = 1;
    for (int col = 0; col < size; col++) {
        int minor[MAX_SIZE][MAX_SIZE];
        int minor_i = 0;
        for (int i = 1; i < size; i++) {
            int minor_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                minor[minor_i][minor_j] = matrix[i][j];
                minor_j++;
            }
            minor_i++;
        }
        int minor_det = recursive_determinant(minor, size - 1);
        det += sign * matrix[0][col] * minor_det;
        sign = -sign;
    }
    return det;
}

int main() {
    int matrix[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols) and elements:\n");
    if (!read_matrix(matrix, &rows, &cols)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, rows, cols);
    
    if (rows == cols) {
        int trace = recursive_trace(matrix, 0, rows, 0);
        printf("\nTrace: %d\n", trace);
        
        int det = recursive_determinant(matrix, rows);
        printf("Determinant: %d\n", det);
    }
    
    int transposed[MAX_SIZE][MAX_SIZE];
    if (recursive_transpose(matrix, transposed, 0, 0, rows, cols)) {
        printf("\nTransposed matrix:\n");
        print_matrix(transposed, cols, rows);
    }
    
    return 0;
}