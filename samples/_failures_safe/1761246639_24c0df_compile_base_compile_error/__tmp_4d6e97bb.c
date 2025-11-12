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

int main() {
    int rows1, cols1, rows2, cols2;
    int op_choice;
    
    printf("Enter rows and columns for first matrix: ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    int matrix1[10][10];
    printf("Enter %d elements for first matrix:\n", rows1 * cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose, 4=Determinant): ");
    if (scanf("%d", &op_choice) != 1 || op_choice < 0 || op_choice > 4) {
        printf("Invalid operation\n");
        return 1;
    }
    
    enum Operation op = (enum Operation)op_choice;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter rows and columns for second matrix: ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            
            int matrix2[10][10];
            printf("Enter %d elements for second matrix:\n", rows2 * cols2);
            for (int i = 0; i < rows2; i++) {
                for (int j = 0; j < cols2; j++) {
                    if (scanf("%d", &matrix2[i][j]) != 1) {
                        printf("Invalid input\n");
                        return 1;
                    }
                }
            }
            
            int result[10][10];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (op == OP_ADD) {
                        result[i][j] = matrix1[i][j] + matrix2[i][j];
                    } else {
                        result[i][j] = matrix1[i][j] - matrix2[i][j];
                    }
                }
            }
            
            printf("Result:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case OP_MULTIPLY:
            printf("Enter rows and columns for second matrix: ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (cols1 != rows2) {
                printf("Matrix dimensions incompatible for multiplication\n");
                return 1;
            }
            
            int matrix2_mul[10][10];
            printf("Enter %d elements for second matrix:\n", rows2 * cols2);
            for (int i = 0; i < rows2; i++) {
                for (int j = 0; j < cols2; j++) {
                    if (scanf("%d", &matrix2_mul[i][j]) != 1) {
                        printf