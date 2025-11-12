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
    int rows1, cols1, rows2, cols2;
    enum Operation op;
    
    printf("Enter matrix 1 dimensions (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[rows1][cols1];
    printf("Enter matrix 1 elements:\n");
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose): ");
    int op_input;
    if (scanf("%d", &op_input) != 1) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (op_input < 0 || op_input > 3) {
        printf("Invalid operation\n");
        return 1;
    }
    op = (enum Operation)op_input;
    
    if (op == OP_TRANSPOSE) {
        int result[cols1][rows1];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                result[j][i] = matrix1[i][j];
            }
        }
        printf("Transpose result:\n");
        print_matrix(cols1, rows1, result);
        return 0;
    }
    
    printf("Enter matrix 2 dimensions (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix2[rows2][cols2];
    printf("Enter matrix 2 elements:\n");
    if (!read_matrix(rows2, cols2, matrix2)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    if (op == OP_ADD) {
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Dimension mismatch for addition\n");
            return 1;
        }
        int sum[rows1][cols1];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                sum[i][j] = matrix1[i][j] + matrix2[i][j];
            }
        }
        printf("Addition result:\n");
        print_matrix(rows1, cols1, sum);
    } else if (op == OP_SUBTRACT) {
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Dimension mismatch for subtraction\n");
            return 1;
        }
        int diff[rows1][cols1];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                diff[i][j] = matrix1[i][j] - matrix2[i][j];
            }
        }
        printf("Subtraction result:\n");
        print_matrix(rows1, cols1, diff);
    } else if (op == OP_MULTIPLY) {
        if (cols1 != rows2) {
            printf("Dimension mismatch for multiplication\n");
            return 1;
        }
        int product[rows1][cols2];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                product[i][j] = 0;
                for (int k = 0; k < cols1; k++) {
                    product[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
        printf("Multiplication result:\n");
        print_matrix(rows1, cols2, product);
    } else {
        printf("Invalid operation\n");
        return 1;
    }
    
    return 0;
}