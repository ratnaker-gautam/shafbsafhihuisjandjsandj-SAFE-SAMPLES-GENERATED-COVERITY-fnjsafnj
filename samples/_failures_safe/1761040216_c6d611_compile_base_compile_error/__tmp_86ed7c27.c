//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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

int main(void) {
    int rows1, cols1, rows2, cols2;
    enum Operation op;
    
    printf("Enter matrix 1 rows and columns: ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    int matrix1[10][10];
    printf("Enter matrix 1 elements:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose, 4=determinant): ");
    int op_input;
    if (scanf("%d", &op_input) != 1 || op_input < 0 || op_input > 4) {
        printf("Invalid operation\n");
        return 1;
    }
    op = (enum Operation)op_input;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY:
            printf("Enter matrix 2 rows and columns: ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            
            int matrix2[10][10];
            printf("Enter matrix 2 elements:\n");
            for (int i = 0; i < rows2; i++) {
                for (int j = 0; j < cols2; j++) {
                    if (scanf("%d", &matrix2[i][j]) != 1) {
                        printf("Invalid input\n");
                        return 1;
                    }
                }
            }
            
            switch (op) {
                case OP_ADD:
                    if (rows1 != rows2 || cols1 != cols2) {
                        printf("Matrix dimensions must match for addition\n");
                        return 1;
                    }
                    printf("Result:\n");
                    for (int i = 0; i < rows1; i++) {
                        for (int j = 0; j < cols1; j++) {
                            printf("%d ", matrix1[i][j] + matrix2[i][j]);
                        }
                        printf("\n");
                    }
                    break;
                    
                case OP_SUBTRACT:
                    if (rows1 != rows2 || cols1 != cols2) {
                        printf("Matrix dimensions must match for subtraction\n");
                        return 1;
                    }
                    printf("Result:\n");
                    for (int i = 0; i < rows1; i++) {
                        for (int j = 0; j < cols1; j++) {
                            printf("%d ", matrix1[i][j] - matrix2[i][j]);
                        }
                        printf("\n");
                    }
                    break;
                    
                case OP_MULTIPLY:
                    if (cols1 != rows2) {
                        printf("Invalid dimensions for multiplication\n");
                        return 1;
                    }
                    printf("Result:\n");
                    for (int i = 0; i < rows1; i++) {
                        for (int j = 0; j < cols2; j++) {
                            int sum = 0;
                            for (int k = 0; k < cols1; k++) {
                                sum += matrix1[i][k] * matrix2[k][j];
                            }
                            printf("%d ", sum);
                        }
                        printf("\n");
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        case OP_TRANSPOSE:
            printf("Transpose:\n");
            for (int j = 0; j < cols1; j++) {
                for (int i = 0; i < rows1; i++) {
                    printf("%d ", matrix1[i][j]);
                }
                printf("\n");
            }
            break;
            
        case OP_DETERMINANT:
            if (rows1 != cols1) {
                printf("Matrix must be square for determinant\n");
                return 1;
            }
            if (rows1 == 1) {
                printf("Determinant: %d\n", matrix1[0][0]);
            } else if (rows1 == 2) {
                int det = matrix1[0][0] * matrix1[1][1] - matrix1[0][1] * matrix1[1][0];
                printf("Determinant: %d\n", det);
            } else {
                printf("Determinant calculation only supported for 1x1