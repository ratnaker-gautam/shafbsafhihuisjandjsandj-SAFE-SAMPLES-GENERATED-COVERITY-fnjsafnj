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
    int op_choice;
    
    printf("Enter number of rows for matrix 1: ");
    if (scanf("%d", &rows1) != 1 || rows1 <= 0 || rows1 > 100) {
        printf("Invalid rows for matrix 1\n");
        return 1;
    }
    
    printf("Enter number of columns for matrix 1: ");
    if (scanf("%d", &cols1) != 1 || cols1 <= 0 || cols1 > 100) {
        printf("Invalid columns for matrix 1\n");
        return 1;
    }
    
    printf("Enter matrix 1 elements:\n");
    int matrix1[rows1][cols1];
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Failed to read matrix 1\n");
        return 1;
    }
    
    printf("Enter operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose): ");
    if (scanf("%d", &op_choice) != 1 || op_choice < 0 || op_choice > 3) {
        printf("Invalid operation choice\n");
        return 1;
    }
    
    enum Operation op = (enum Operation)op_choice;
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        printf("Enter number of rows for matrix 2: ");
        if (scanf("%d", &rows2) != 1 || rows2 <= 0 || rows2 > 100) {
            printf("Invalid rows for matrix 2\n");
            return 1;
        }
        
        printf("Enter number of columns for matrix 2: ");
        if (scanf("%d", &cols2) != 1 || cols2 <= 0 || cols2 > 100) {
            printf("Invalid columns for matrix 2\n");
            return 1;
        }
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Matrix dimensions must match for addition/subtraction\n");
            return 1;
        }
        
        printf("Enter matrix 2 elements:\n");
        int matrix2[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Failed to read matrix 2\n");
            return 1;
        }
        
        int result[rows1][cols1];
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
    } else if (op == OP_MULTIPLY) {
        printf("Enter number of rows for matrix 2: ");
        if (scanf("%d", &rows2) != 1 || rows2 <= 0 || rows2 > 100) {
            printf("Invalid rows for matrix 2\n");
            return 1;
        }
        
        printf("Enter number of columns for matrix 2: ");
        if (scanf("%d", &cols2) != 1 || cols2 <= 0 || cols2 > 100) {
            printf("Invalid columns for matrix 2\n");
            return 1;
        }
        
        if (cols1 != rows2) {
            printf("Matrix 1 columns must equal matrix 2 rows for multiplication\n");
            return 1;
        }
        
        printf("Enter matrix 2 elements:\n");
        int matrix2_mult[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2_mult)) {
            printf("Failed to read matrix 2\n");
            return 1;
        }
        
        int result_mult[rows1][cols2];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                result_mult[i][j] = 0;
                for (int k = 0; k < cols1; k++) {
                    result_mult[i][j] += matrix1[i][k] * matrix2_mult[k][j];
                }
            }
        }
        printf