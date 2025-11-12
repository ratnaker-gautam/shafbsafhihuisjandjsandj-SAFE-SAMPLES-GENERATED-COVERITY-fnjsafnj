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
            printf("%d", matrix[i][j]);
            if (j < cols - 1) printf(" ");
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

int main(void) {
    enum Operation op;
    int op_input;
    int rows1, cols1, rows2, cols2;
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose): ");
    if (scanf("%d", &op_input) != 1) {
        printf("Invalid operation input\n");
        return 1;
    }
    
    if (op_input < 0 || op_input > 3) {
        printf("Invalid operation\n");
        return 1;
    }
    op = (enum Operation)op_input;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter rows and columns for both matrices: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            rows2 = rows1;
            cols2 = cols1;
            break;
            
        case OP_MULTIPLY:
            printf("Enter rows and columns for first matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            printf("Enter rows and columns for second matrix: ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (cols1 != rows2) {
                printf("Matrix dimensions incompatible for multiplication\n");
                return 1;
            }
            break;
            
        case OP_TRANSPOSE:
            printf("Enter rows and columns for matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            rows2 = 0;
            cols2 = 0;
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    int matrix1[100][100];
    int matrix2[100][100];
    int result[100][100];
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter first matrix (%dx%d):\n", rows1, cols1);
            if (!read_matrix(rows1, cols1, matrix1)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            printf("Enter second matrix (%dx%d):\n", rows2, cols2);
            if (!read_matrix(rows2, cols2, matrix2)) {
                printf("Invalid matrix input\n");
                return 1;
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
            printf("Enter first matrix (%dx%d):\n", rows1, cols1);
            if (!read_matrix(rows1, cols1, matrix1)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            printf("Enter second matrix (%dx%d):\n", rows2, cols2);
            if (!read_matrix(rows2, cols2, matrix2)) {
                printf("