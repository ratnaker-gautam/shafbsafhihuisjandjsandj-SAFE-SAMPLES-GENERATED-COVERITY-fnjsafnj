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

int determinant_2x2(int a, int b, int c, int d) {
    return a * d - b * c;
}

int determinant_3x3(int matrix[3][3]) {
    int det = 0;
    det += matrix[0][0] * determinant_2x2(matrix[1][1], matrix[1][2], matrix[2][1], matrix[2][2]);
    det -= matrix[0][1] * determinant_2x2(matrix[1][0], matrix[1][2], matrix[2][0], matrix[2][2]);
    det += matrix[0][2] * determinant_2x2(matrix[1][0], matrix[1][1], matrix[2][0], matrix[2][1]);
    return det;
}

int main() {
    enum Operation op;
    int rows1, cols1, rows2, cols2;
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose, 4=determinant): ");
    if (scanf("%d", (int*)&op) != 1 || op < 0 || op > 4) {
        printf("Invalid operation\n");
        return 1;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY:
            printf("Enter rows and columns for first matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 10 || cols1 > 10) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            
            int mat1[10][10];
            printf("Enter %d elements for first matrix:\n", rows1 * cols1);
            if (!read_matrix(rows1, cols1, mat1)) {
                printf("Invalid input\n");
                return 1;
            }
            
            printf("Enter rows and columns for second matrix: ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > 10 || cols2 > 10) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            
            int mat2[10][10];
            printf("Enter %d elements for second matrix:\n", rows2 * cols2);
            if (!read_matrix(rows2, cols2, mat2)) {
                printf("Invalid input\n");
                return 1;
            }
            
            int result[10][10];
            
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
                    printf("Result:\n");
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
                    printf("Result:\n");
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
                    printf("Result:\n");
                    print_matrix(rows1, cols2, result);
                    break;
                    
                default:
                    break;
            }
            break;
            
        case OP_TRANSPOSE