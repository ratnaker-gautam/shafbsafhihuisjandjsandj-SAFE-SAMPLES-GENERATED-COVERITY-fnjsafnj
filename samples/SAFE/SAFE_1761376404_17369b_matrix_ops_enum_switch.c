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

enum Operation get_operation() {
    int choice;
    if (scanf("%d", &choice) != 1) {
        return OP_INVALID;
    }
    if (choice < 0 || choice > 3) {
        return OP_INVALID;
    }
    return (enum Operation)choice;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    int matrix1[10][10];
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix data\n");
        return 1;
    }
    
    printf("Select operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose): ");
    enum Operation op = get_operation();
    if (op == OP_INVALID) {
        printf("Invalid operation\n");
        return 1;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY:
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            
            int matrix2[10][10];
            if (!read_matrix(rows2, cols2, matrix2)) {
                printf("Invalid matrix data\n");
                return 1;
            }
            
            if (op == OP_ADD || op == OP_SUBTRACT) {
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Matrix dimensions must match for addition/subtraction\n");
                    return 1;
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
            } else {
                if (cols1 != rows2) {
                    printf("Invalid dimensions for multiplication\n");
                    return 1;
                }
                
                int result[10][10];
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
            }
            break;
            
        case OP_TRANSPOSE:
            int transpose[10][10];
            for (int i = 0; i < cols1; i++) {
                for (int j = 0; j < rows1; j++) {
                    transpose[i][j] = matrix1[j][i];
                }
            }
            printf("Result:\n");
            print_matrix(cols1, rows1, transpose);
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}