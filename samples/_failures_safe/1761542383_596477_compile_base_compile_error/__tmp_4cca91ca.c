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

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return 1;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return 1;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int op_choice;
    
    printf("Enter number of rows for matrix 1: ");
    if (scanf("%d", &rows1) != 1 || rows1 <= 0 || rows1 > 100) {
        printf("Invalid row count\n");
        return 1;
    }
    
    printf("Enter number of columns for matrix 1: ");
    if (scanf("%d", &cols1) != 1 || cols1 <= 0 || cols1 > 100) {
        printf("Invalid column count\n");
        return 1;
    }
    
    printf("Enter number of rows for matrix 2: ");
    if (scanf("%d", &rows2) != 1 || rows2 <= 0 || rows2 > 100) {
        printf("Invalid row count\n");
        return 1;
    }
    
    printf("Enter number of columns for matrix 2: ");
    if (scanf("%d", &cols2) != 1 || cols2 <= 0 || cols2 > 100) {
        printf("Invalid column count\n");
        return 1;
    }
    
    printf("Select operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose): ");
    if (scanf("%d", &op_choice) != 1 || op_choice < 0 || op_choice > 3) {
        printf("Invalid operation\n");
        return 1;
    }
    
    enum Operation op = (enum Operation)op_choice;
    
    int matrix1[rows1][cols1];
    int matrix2[rows2][cols2];
    
    printf("Enter elements of matrix 1 (%dx%d):\n", rows1, cols1);
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    if (op != OP_TRANSPOSE) {
        printf("Enter elements of matrix 2 (%dx%d):\n", rows2, cols2);
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid matrix input\n");
            return 1;
        }
    }
    
    switch (op) {
        case OP_ADD: {
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            
            int result[rows1][cols1];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (!safe_add(matrix1[i][j], matrix2[i][j])) {
                        printf("Integer overflow in addition\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            printf("Result of addition:\n");
            print_matrix(rows1, cols1, result);
            break;
        }
        
        case OP_SUBTRACT: {
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for subtraction\n");
                return 1;
            }
            
            int result[rows1][cols1];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (!safe_add(matrix1[i][j], -matrix2[i][j])) {
                        printf("Integer overflow in subtraction\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i