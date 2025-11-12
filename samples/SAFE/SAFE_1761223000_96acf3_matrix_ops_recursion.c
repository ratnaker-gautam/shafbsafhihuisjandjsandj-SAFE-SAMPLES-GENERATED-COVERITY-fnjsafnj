//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 8

int read_matrix(int matrix[MAX_DIM][MAX_DIM], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(int matrix[MAX_DIM][MAX_DIM], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_recursive(int src[MAX_DIM][MAX_DIM], int dst[MAX_DIM][MAX_DIM], int i, int j, int rows, int cols) {
    if (i >= rows) {
        return;
    }
    if (j >= cols) {
        transpose_recursive(src, dst, i + 1, 0, rows, cols);
        return;
    }
    dst[j][i] = src[i][j];
    transpose_recursive(src, dst, i, j + 1, rows, cols);
}

int determinant_recursive(int matrix[MAX_DIM][MAX_DIM], int n) {
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    int det = 0;
    int sign = 1;
    int temp[MAX_DIM][MAX_DIM];
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
        det += sign * matrix[0][col] * determinant_recursive(temp, n - 1);
        sign = -sign;
    }
    return det;
}

int main() {
    int choice;
    printf("Matrix Operations:\n1. Transpose\n2. Determinant\nEnter choice: ");
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice\n");
        return 1;
    }

    int rows, cols;
    printf("Enter rows and columns: ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        printf("Dimensions out of range (1-%d)\n", MAX_DIM);
        return 1;
    }

    if (choice == 2 && rows != cols) {
        printf("Determinant requires square matrix\n");
        return 1;
    }

    int matrix[MAX_DIM][MAX_DIM];
    printf("Enter matrix elements:\n");
    if (!read_matrix(matrix, rows, cols)) {
        printf("Invalid matrix input\n");
        return 1;
    }

    if (choice == 1) {
        int transposed[MAX_DIM][MAX_DIM];
        transpose_recursive(matrix, transposed, 0, 0, rows, cols);
        printf("Transposed matrix:\n");
        print_matrix(transposed, cols, rows);
    } else {
        int det = determinant_recursive(matrix, rows);
        printf("Determinant: %d\n", det);
    }

    return 0;
}