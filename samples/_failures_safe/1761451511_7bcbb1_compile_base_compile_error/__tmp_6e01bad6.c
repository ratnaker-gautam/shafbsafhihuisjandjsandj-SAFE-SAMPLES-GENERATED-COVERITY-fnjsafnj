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

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int op_choice;
    enum Operation op;
    
    printf("Enter first matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
        printf("Dimensions must be between 1 and 10\n");
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
    
    printf("Enter operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose): ");
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (op_choice < 0 || op_choice > 3) {
        op = OP_INVALID;
    } else {
        op = (enum Operation)op_choice;
    }
    
    int result[10][10];
    int matrix2[10][10];
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter second matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (rows2 != rows1 || cols2 != cols1) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            
            printf("Enter %d elements for second matrix:\n", rows2 * cols2);
            for (int i = 0; i < rows2; i++) {
                for (int j = 0; j < cols2; j++) {
                    if (scanf("%d", &matrix2[i][j]) != 1) {
                        printf("Invalid input\n");
                        return 1;
                    }
                }
            }
            
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
            printf("Enter second matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            if (rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
                printf("Dimensions must be between 1 and 10\n");
                return 1;
            }
            
            printf("Enter %d elements for second matrix:\n", rows2 * cols2);
            for (int i = 0; i < rows2; i++) {
                for (int j = 0; j < cols2; j++) {
                    if (scanf("%d", &matrix2[i][j]) != 1) {
                        printf("Invalid input\n");
                        return 1;
                    }
                }
            }
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                }
            }
            
            printf("Result:\n");
            print_matrix(rows1, cols2, result);
            break;
            
        case OP_TRANSPOSE:
            for (int i = 0; i < cols1; i++) {
                for (int j = 0; j < rows1; j++) {
                    result[i][j] = matrix1[j][i];
                }
            }
            
            printf("Result:\n");
            print_matrix(cols1, rows1, result