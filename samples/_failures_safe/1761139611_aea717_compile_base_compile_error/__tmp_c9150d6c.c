//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

int determinant(int n, int matrix[n][n]) {
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    int det = 0;
    int sign = 1;
    int temp[10][10];
    
    for (int k = 0; k < n; k++) {
        int sub_i = 0;
        for (int i = 1; i < n; i++) {
            int sub_j = 0;
            for (int j = 0; j < n; j++) {
                if (j == k) continue;
                temp[sub_i][sub_j] = matrix[i][j];
                sub_j++;
            }
            sub_i++;
        }
        det += sign * matrix[0][k] * determinant(n - 1, temp);
        sign = -sign;
    }
    return det;
}

int main(void) {
    enum Operation op;
    int rows1, cols1, rows2, cols2;
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose, 4=determinant): ");
    if (scanf("%d", (int*)&op) != 1 || op < OP_ADD || op > OP_DETERMINANT) {
        printf("Invalid operation\n");
        return 1;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter rows and columns for both matrices: ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid dimensions\n");
                return 1;
            }
            rows2 = rows1;
            cols2 = cols1;
            break;
            
        case OP_MULTIPLY:
            printf("Enter rows and columns for first matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid dimensions\n");
                return 1;
            }
            printf("Enter rows and columns for second matrix: ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (cols1 != rows2) {
                printf("Dimension mismatch for multiplication\n");
                return 1;
            }
            break;
            
        case OP_TRANSPOSE:
        case OP_DETERMINANT:
            printf("Enter rows and columns: ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (op == OP_DETERMINANT && rows1 != cols1) {
                printf("Determinant requires square matrix\n");
                return 1;
            }
            rows2 = 0;
            cols2 = 0;
            break;
    }
    
    int mat1[100][100];
    int mat2[100][100];
    int result[100][100];
    
    printf("Enter first matrix elements:\n");
    if (!read_matrix(rows1, cols1, mat1)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    if (op == OP_ADD || op == OP_SUBTRACT || op == OP_MULTIPLY) {
        printf("Enter second matrix elements:\n");
        if (!read_matrix(rows2, cols2, mat2)) {
            printf("Invalid matrix input\n");
            return 1;
        }
    }
    
    switch (op) {
        case OP_ADD:
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    result[i][j] = mat1[i][j] + mat2[i][j];
                }
            }
            printf("Result:\n");
            print_matrix(rows1,