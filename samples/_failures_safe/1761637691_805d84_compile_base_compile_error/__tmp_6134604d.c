//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

enum operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
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

int validate_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > 10 || cols > 10) {
        printf("Invalid dimensions: must be between 1 and 10\n");
        return 0;
    }
    return 1;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    enum operation op;
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose, 4=determinant): ");
    if (scanf("%d", (int*)&op) != 1 || op < 0 || op > 4) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (op == OP_TRANSPOSE || op == OP_DETERMINANT) {
        printf("Enter matrix rows and columns: ");
        if (scanf("%d %d", &rows1, &cols1) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (!validate_dimensions(rows1, cols1)) return 1;
        
        if (op == OP_DETERMINANT && rows1 != cols1) {
            printf("Determinant requires square matrix\n");
            return 1;
        }
        
        int matrix1[rows1][cols1];
        read_matrix(rows1, cols1, matrix1);
        
        switch (op) {
            case OP_TRANSPOSE: {
                int result[cols1][rows1];
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                        result[j][i] = matrix1[i][j];
                    }
                }
                printf("Transpose:\n");
                print_matrix(cols1, rows1, result);
                break;
            }
            case OP_DETERMINANT: {
                if (rows1 == 1) {
                    printf("Determinant: %d\n", matrix1[0][0]);
                } else if (rows1 == 2) {
                    int det = matrix1[0][0] * matrix1[1][1] - matrix1[0][1] * matrix1[1][0];
                    printf("Determinant: %d\n", det);
                } else if (rows1 == 3) {
                    int det = matrix1[0][0] * (matrix1[1][1] * matrix1[2][2] - matrix1[1][2] * matrix1[2][1]) -
                              matrix1[0][1] * (matrix1[1][0] * matrix1[2][2] - matrix1[1][2] * matrix1[2][0]) +
                              matrix1[0][2] * (matrix1[1][0] * matrix1[2][1] - matrix1[1][1] * matrix1[2][0]);
                    printf("Determinant: %d\n", det);
                } else {
                    printf("Determinant calculation only supported for 1x1, 2x2, and 3x3 matrices\n");
                    return 1;
                }
                break;
            }
            default:
                break;
        }
    } else {
        printf("Enter first matrix rows and columns: ");
        if (scanf("%d %d", &rows1, &cols1) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (!validate_dimensions(rows1, cols1)) return 1;
        
        printf("Enter second matrix rows and columns: ");
        if (scanf("%d %d", &rows2, &cols2) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (!validate_dimensions(rows2, cols2)) return 1;
        
        if ((op == OP_ADD || op == OP_SUBTRACT) && (rows1 != rows2 || cols1 != cols2)) {
            printf("Matrix dimensions must match for addition/subtraction\n");
            return 1;
        }
        if (op == OP_MULTIPLY && cols1 != rows2) {
            printf("Invalid dimensions for multiplication\n");
            return 1;
        }
        
        int matrix1[rows1][cols1];
        int matrix2[rows2][cols2];
        
        printf("First