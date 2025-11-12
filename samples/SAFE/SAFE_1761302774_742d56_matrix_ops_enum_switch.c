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
    OP_INVALID
};

int read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    enum Operation op;
    int choice;
    
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions of second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > 10 || cols1 <= 0 || cols2 > 10) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose first matrix\n");
    printf("Enter choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    op = (enum Operation)choice;
    
    int mat1[10][10];
    int mat2[10][10];
    int result[10][10];
    
    printf("Enter first matrix elements:\n");
    if (!read_matrix(rows1, cols1, mat1)) {
        printf("Error reading matrix\n");
        return 1;
    }
    
    if (op != OP_TRANSPOSE) {
        printf("Enter second matrix elements:\n");
        if (!read_matrix(rows2, cols2, mat2)) {
            printf("Error reading matrix\n");
            return 1;
        }
    }
    
    switch (op) {
        case OP_ADD:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    result[i][j] = mat1[i][j] + mat2[i][j];
                }
            }
            printf("Result of addition:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case OP_SUBTRACT:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for subtraction\n");
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    result[i][j] = mat1[i][j] - mat2[i][j];
                }
            }
            printf("Result of subtraction:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case OP_MULTIPLY:
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += mat1[i][k] * mat2[k][j];
                    }
                }
            }
            printf("Result of multiplication:\n");
            print_matrix(rows1, cols2, result);
            break;
            
        case OP_TRANSPOSE:
            for (int i = 0; i < cols1; i++) {
                for (int j = 0; j < rows1; j++) {
                    result[i][j] = mat1[j][i];
                }
            }
            printf("Transpose of first matrix:\n");
            print_matrix(cols1, rows1, result);
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}