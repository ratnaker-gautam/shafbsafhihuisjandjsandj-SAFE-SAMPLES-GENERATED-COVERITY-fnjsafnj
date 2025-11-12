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
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols2 > 100) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[rows1][cols1];
    printf("Enter matrix 1 elements:\n");
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
    
    switch (op_input) {
        case 0: op = OP_ADD; break;
        case 1: op = OP_SUBTRACT; break;
        case 2: op = OP_MULTIPLY; break;
        case 3: op = OP_TRANSPOSE; break;
        default: op = OP_INVALID; break;
    }
    
    if (op == OP_INVALID) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (op != OP_TRANSPOSE) {
        printf("Enter matrix 2 dimensions (rows cols): ");
        if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        printf("Enter matrix 2 elements:\n");
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid matrix elements\n");
            return 1;
        }
        
        if (op == OP_ADD || op == OP_SUBTRACT) {
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            int result_add_sub[rows1][cols1];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (op == OP_ADD) {
                        result_add_sub[i][j] = matrix1[i][j] + matrix2[i][j];
                    } else {
                        result_add_sub[i][j] = matrix1[i][j] - matrix2[i][j];
                    }
                }
            }
            printf("Result:\n");
            print_matrix(rows1, cols1, result_add_sub);
        } else if (op == OP_MULTIPLY) {
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            int result_mult[rows1][cols2];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result_mult[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result_mult[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                }
            }
            printf("Result:\n");
            print_matrix(rows1, cols2, result_mult);
        }
    } else {
        int result_trans[cols1][rows1];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                result_trans[j][i] = matrix1[i][j];
            }
        }
        printf("Result:\n");
        print_matrix(cols1, rows1, result_trans);
    }
    
    return 0;
}