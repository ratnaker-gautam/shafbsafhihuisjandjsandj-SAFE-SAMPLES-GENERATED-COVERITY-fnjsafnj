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

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > 100 || cols1 <= 0 || cols1 > 100) {
        fprintf(stderr, "Matrix dimensions out of bounds\n");
        return 1;
    }
    
    int matrix1[rows1][cols1];
    if (!read_matrix(rows1, cols1, matrix1)) {
        fprintf(stderr, "Failed to read matrix 1\n");
        return 1;
    }
    
    int op_choice;
    if (scanf("%d", &op_choice) != 1) {
        fprintf(stderr, "Invalid operation choice\n");
        return 1;
    }
    
    enum Operation op = OP_INVALID;
    if (op_choice >= 0 && op_choice <= 3) {
        op = (enum Operation)op_choice;
    }
    
    if (op == OP_ADD || op == OP_SUBTRACT || op == OP_MULTIPLY) {
        if (scanf("%d %d", &rows2, &cols2) != 2) {
            fprintf(stderr, "Invalid second matrix dimensions\n");
            return 1;
        }
        if (rows2 <= 0 || rows2 > 100 || cols2 <= 0 || cols2 > 100) {
            fprintf(stderr, "Second matrix dimensions out of bounds\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2)) {
            fprintf(stderr, "Failed to read matrix 2\n");
            return 1;
        }
        
        if (op == OP_ADD || op == OP_SUBTRACT) {
            if (rows1 != rows2 || cols1 != cols2) {
                fprintf(stderr, "Matrix dimensions must match for addition/subtraction\n");
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
            print_matrix(rows1, cols1, result);
        } else {
            if (cols1 != rows2) {
                fprintf(stderr, "Invalid dimensions for multiplication\n");
                return 1;
            }
            
            int result[rows1][cols2];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                }
            }
            print_matrix(rows1, cols2, result);
        }
    } else if (op == OP_TRANSPOSE) {
        int result[cols1][rows1];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                result[j][i] = matrix1[i][j];
            }
        }
        print_matrix(cols1, rows1, result);
    } else if (op == OP_INVALID) {
        fprintf(stderr, "Invalid operation selected\n");
        return 1;
    }
    
    return 0;
}