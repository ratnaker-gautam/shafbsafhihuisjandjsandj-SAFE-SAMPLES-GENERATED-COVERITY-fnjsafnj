//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int is_valid_matrix_size(int rows, int cols) {
    return rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE;
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_recursive(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int i, int j) {
    if (i >= rows) return;
    if (j >= cols) {
        transpose_recursive(rows, cols, src, dest, i + 1, 0);
        return;
    }
    dest[j][i] = src[i][j];
    transpose_recursive(rows, cols, src, dest, i, j + 1);
}

int determinant_recursive(int n, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (n == 1) return matrix[0][0];
    if (n == 2) return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    
    int det = 0;
    int sign = 1;
    int temp[MAX_SIZE][MAX_SIZE];
    
    for (int col = 0; col < n; col++) {
        int sub_i = 0;
        for (int i = 1; i < n; i++) {
            int sub_j = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                temp[sub_i][sub_j] = matrix[i][j];
                sub_j++;
            }
            sub_i++;
        }
        det += sign * matrix[0][col] * determinant_recursive(n - 1, temp);
        sign = -sign;
    }
    return det;
}

int main() {
    int choice;
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Menu:\n");
    printf("1. Transpose\n");
    printf("2. Determinant\n");
    printf("Enter choice (1-2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!is_valid_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    if (choice == 2 && rows != cols) {
        printf("Determinant requires square matrix\n");
        return 1;
    }
    
    read_matrix(rows, cols, matrix);
    
    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);
    
    if (choice == 1) {
        memset(result, 0, sizeof(result));
        transpose_recursive(rows, cols, matrix, result, 0, 0);
        printf("\nTransposed matrix:\n");
        print_matrix(cols, rows, result);
    } else if (choice == 2) {
        int det = determinant_recursive(rows, matrix);
        printf("\nDeterminant: %d\n", det);
    }
    
    return 0;
}