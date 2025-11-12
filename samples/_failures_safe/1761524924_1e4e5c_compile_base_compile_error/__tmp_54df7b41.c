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
    
    if (rows1 > 100 || cols1 > 100) {
        printf("Dimensions too large\n");
        return 1;
    }
    
    printf("Enter matrix 1 elements:\n");
    int matrix1[rows1][cols1];
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix elements\n");
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
    
    int rows_result, cols_result;
    int result[100][100];
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        printf("Enter matrix 2 dimensions (rows cols): ");
        if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        if (rows2 > 100 || cols2 > 100) {
            printf("Dimensions too large\n");
            return 1;
        }
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Dimension mismatch for addition/subtraction\n");
            return 1;
        }
        printf("Enter matrix 2 elements:\n");
        int matrix2[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid matrix elements\n");
            return 1;
        }
        rows_result = rows1;
        cols_result = cols1;
        for (int i = 0; i < rows_result; i++) {
            for (int j = 0; j < cols_result; j++) {
                if (op == OP_ADD) {
                    if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                        (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                } else {
                    if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
        }
    } else if (op == OP_MULTIPLY) {
        printf("Enter matrix 2 dimensions (rows cols): ");
        if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        if (rows2 > 100 || cols2 > 100) {
            printf("Dimensions too large\n");
            return 1;
        }
        
        if (cols1 != rows2) {
            printf("Dimension mismatch for multiplication\n");
            return 1;
        }
        printf("Enter matrix 2 elements:\n");
        int matrix2_mult[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2_mult)) {
            printf("Invalid matrix elements\n");
            return 1;
        }
        rows_result = rows1;
        cols_result = cols2;
        for (int i = 0; i < rows_result; i++) {
            for (int j = 0; j < cols_result; j++) {
                long long sum