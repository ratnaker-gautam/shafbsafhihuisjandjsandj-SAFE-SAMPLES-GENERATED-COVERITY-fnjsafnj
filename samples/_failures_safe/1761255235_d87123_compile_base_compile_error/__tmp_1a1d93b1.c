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

int main(void) {
    enum Operation op;
    int op_choice;
    int rows1, cols1, rows2, cols2;
    
    printf("Enter matrix operation (0=add, 1=subtract, 2=multiply, 3=transpose): ");
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid operation choice\n");
        return 1;
    }
    
    if (op_choice < 0 || op_choice > 3) {
        printf("Invalid operation choice\n");
        return 1;
    }
    op = (enum Operation)op_choice;
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        printf("Enter dimensions for both matrices (rows cols): ");
        if (scanf("%d %d", &rows1, &cols1) != 2) {
            printf("Invalid dimensions\n");
            return 1;
        }
        if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        int mat1[rows1][cols1];
        int mat2[rows1][cols1];
        int result[rows1][cols1];
        
        printf("Enter first matrix (%dx%d):\n", rows1, cols1);
        if (!read_matrix(rows1, cols1, mat1)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        printf("Enter second matrix (%dx%d):\n", rows1, cols1);
        if (!read_matrix(rows1, cols1, mat2)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                if (op == OP_ADD) {
                    result[i][j] = mat1[i][j] + mat2[i][j];
                } else {
                    result[i][j] = mat1[i][j] - mat2[i][j];
                }
            }
        }
        
        printf("Result:\n");
        print_matrix(rows1, cols1, result);
    } else if (op == OP_MULTIPLY) {
        printf("Enter dimensions for first matrix (rows cols): ");
        if (scanf("%d %d", &rows1, &cols1) != 2) {
            printf("Invalid dimensions\n");
            return 1;
        }
        if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        printf("Enter dimensions for second matrix (rows cols): ");
        if (scanf("%d %d", &rows2, &cols2) != 2) {
            printf("Invalid dimensions\n");
            return 1;
        }
        if (rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        if (cols1 != rows2) {
            printf("Matrix dimensions incompatible for multiplication\n");
            return 1;
        }
        
        int matA[rows1][cols1];
        int matB[rows2][cols2];
        int product[rows1][cols2];
        
        printf("Enter first matrix (%dx%d):\n", rows1, cols1);
        if (!read_matrix(rows1, cols1, matA)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        printf("Enter second matrix (%dx%d):\n", rows2, cols2);
        if (!read_matrix(rows2, cols2, matB)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                product[i][j] = 0;
                for (int k = 0; k < cols1; k++) {
                    product[i][j] += matA[i][k] * matB[k][j];
                }
            }
        }
        
        printf("Result:\n");
        print_matrix(rows1, cols2, product);
    } else if (op == OP_TRANSPOSE) {
        printf("Enter matrix dimensions (rows cols): ");
        if