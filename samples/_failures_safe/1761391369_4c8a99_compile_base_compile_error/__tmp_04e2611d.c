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
            printf("Enter element [%d][%d]: ", i, j);
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
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

enum Operation get_operation() {
    int choice;
    printf("Select operation:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("Enter choice: ");
    if (scanf("%d", &choice) != 1) {
        return OP_INVALID;
    }
    if (choice < 0 || choice > 3) {
        return OP_INVALID;
    }
    return (enum Operation)choice;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[rows1][cols1];
    printf("Enter first matrix:\n");
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    enum Operation op = get_operation();
    if (op == OP_INVALID) {
        printf("Invalid operation\n");
        return 1;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter dimensions of second matrix (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (rows2 != rows1 || cols2 != cols1) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            break;
        case OP_MULTIPLY:
            printf("Enter dimensions of second matrix (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            break;
        case OP_TRANSPOSE:
            rows2 = cols1;
            cols2 = rows1;
            break;
        default:
            return 1;
    }
    
    int matrix2[rows2][cols2];
    if (op != OP_TRANSPOSE) {
        printf("Enter second matrix:\n");
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid matrix input\n");
            return 1;
        }
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(rows1, cols1, matrix1);
    
    if (op != OP_TRANSPOSE) {
        printf("\nSecond matrix:\n");
        print_matrix(rows2, cols2, matrix2);
    }
    
    printf("\nResult:\n");
    
    switch (op) {
        case OP_ADD: {
            int result[rows1][cols1];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            print_matrix(rows1, cols1, result);
            break;
        }
        case OP_SUBTRACT: {
            int result[rows1][cols1];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
            print_matrix(rows1, cols1, result);
            break;
        }
        case OP_MULTIPLY: {
            int result[rows1][cols2];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                }
            }
            print