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
    
    printf("Enter matrix 1 elements:\n");
    int matrix1[rows1][cols1];
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix elements\n");
        return 1;
    }
    
    printf("Select operation (0=add, 1=subtract, 2=multiply, 3=transpose): ");
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
        if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
            printf("Invalid dimensions\n");
            return 1;
        }
        
        printf("Enter matrix 2 elements:\n");
        int matrix2[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid matrix elements\n");
            return 1;
        }
        
        switch (op) {
            case OP_ADD:
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Matrix dimensions must match for addition\n");
                    return 1;
                }
                printf("Result:\n");
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                        int sum = matrix1[i][j] + matrix2[i][j];
                        if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                            (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                        printf("%d ", sum);
                    }
                    printf("\n");
                }
                break;
                
            case OP_SUBTRACT:
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Matrix dimensions must match for subtraction\n");
                    return 1;
                }
                printf("Result:\n");
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                        int diff = matrix1[i][j] - matrix2[i][j];
                        if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                            (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                        printf("%d ", diff);
                    }
                    printf("\n");
                }
                break;
                
            case OP_MULTIPLY:
                if (cols1 != rows2) {
                    printf("Invalid dimensions for multiplication\n");
                    return 1;
                }
                printf("Result:\n");
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols2; j++) {
                        long sum = 0;
                        for (int k = 0; k < cols1; k++) {
                            long product = (long)matrix1[i][k] * (long)matrix2[k][j];
                            if (product > INT_MAX || product < INT_MIN) {
                                printf("Integer overflow detected\n");
                                return 1;
                            }
                            sum += product;
                            if (sum > INT_MAX ||