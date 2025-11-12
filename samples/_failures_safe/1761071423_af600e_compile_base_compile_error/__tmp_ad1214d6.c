//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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
            printf("Second matrix too large\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid second matrix data\n");
            return 1;
        }
        
        if (op == OP_ADD) {
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    int sum = matrix1[i][j] + matrix2[i][j];
                    if ((matrix2[i][j] > 0 && sum < matrix1[i][j]) || 
                        (matrix2[i][j] < 0 && sum > matrix1[i][j])) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    printf("%d", sum);
                    if (j < cols1 - 1) printf(" ");
                }
                printf("\n");
            }
        } else if (op == OP_SUBTRACT) {
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for subtraction\n");
                return 1;
            }
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    int diff = matrix1[i][j] - matrix2[i][j];
                    if ((matrix2[i][j] > 0 && diff > matrix1[i][j]) || 
                        (matrix2[i][j] < 0 && diff < matrix1[i][j])) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    printf("%d", diff);
                    if (j < cols1 - 1) printf(" ");
                }
                printf("\n");
            }
        } else if (op == OP_MULTIPLY) {
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    long sum = 0;
                    for (int k = 0; k < cols1; k++) {
                        long product = (long)matrix1[i][k] * matrix2[k][j];
                        if (product > INT_MAX || product < INT_MIN) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                        sum += product;
                        if (sum > INT_MAX || sum < INT_MIN) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                    }
                    printf("%ld", sum);
                    if (j < cols2 - 1) printf(" ");
                }
                printf("\n");
            }
        }
    } else if (op == OP_TRANSPOSE) {
        for (int j = 0; j < cols1; j++) {
            for (int i = 0; i < rows