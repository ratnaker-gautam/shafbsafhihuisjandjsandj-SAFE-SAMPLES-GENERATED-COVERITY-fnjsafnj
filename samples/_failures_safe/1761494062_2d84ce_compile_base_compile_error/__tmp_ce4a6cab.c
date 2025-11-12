//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

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
    int rows1, cols1, rows2, cols2;
    enum Operation op;
    
    if (scanf("%d", &op) != 1 || op < OP_ADD || op > OP_TRANSPOSE) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    if (rows1 > 100 || cols1 > 100) {
        printf("Matrix too large\n");
        return 1;
    }
    
    int matrix1[rows1][cols1];
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix data\n");
        return 1;
    }
    
    if (op == OP_ADD || op == OP_SUBTRACT || op == OP_MULTIPLY) {
        if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
            printf("Invalid second matrix dimensions\n");
            return 1;
        }
        
        if (rows2 > 100 || cols2 > 100) {
            printf("Matrix too large\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid second matrix data\n");
            return 1;
        }
        
        if (op == OP_ADD || op == OP_SUBTRACT) {
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            
            int result[rows1][cols1];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (op == OP_ADD) {
                        if ((matrix2[i][j] > 0 && matrix1[i][j] > INT_MAX - matrix2[i][j]) ||
                            (matrix2[i][j] < 0 && matrix1[i][j] < INT_MIN - matrix2[i][j])) {
                            printf("Integer overflow\n");
                            return 1;
                        }
                        result[i][j] = matrix1[i][j] + matrix2[i][j];
                    } else {
                        if ((matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j]) ||
                            (matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j])) {
                            printf("Integer overflow\n");
                            return 1;
                        }
                        result[i][j] = matrix1[i][j] - matrix2[i][j];
                    }
                }
            }
            print_matrix(rows1, cols1, result);
        } else {
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            
            int result[rows1][cols2];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        long long product = (long long)matrix1[i][k] * matrix2[k][j];
                        if (product > INT_MAX || product < INT_MIN) {
                            printf("Integer overflow\n");
                            return 1;
                        }
                        if ((result[i][j] > 0 && product > INT_MAX - result[i][j]) ||
                            (result[i][j] < 0 && product < INT_MIN - result[i][j])) {
                            printf("Integer overflow\n");
                            return 1;
                        }
                        result[i][j] += (int)product;
                    }
                }
            }
            print_matrix(rows1, cols2, result);
        }
    } else if (op == OP_TRANSPOSE) {
        int result[cols1][rows1];
        for (int i = 0; i < cols1; i++) {
            for (int j = 0; j < rows1; j++) {
                result[i][j] = matrix1[j][i];
            }