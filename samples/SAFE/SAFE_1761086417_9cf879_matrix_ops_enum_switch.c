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
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    int matrix1[rows1][cols1];
    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("Enter dimensions of second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    int matrix2[rows2][cols2];
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("Select operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose): ");
    int op_choice;
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid operation choice\n");
        return 1;
    }

    enum Operation op;
    switch (op_choice) {
        case 0: op = OP_ADD; break;
        case 1: op = OP_SUBTRACT; break;
        case 2: op = OP_MULTIPLY; break;
        case 3: op = OP_TRANSPOSE; break;
        default: op = OP_INVALID; break;
    }

    switch (op) {
        case OP_ADD:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            printf("Result of addition:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    printf("%d ", matrix1[i][j] + matrix2[i][j]);
                }
                printf("\n");
            }
            break;

        case OP_SUBTRACT:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for subtraction\n");
                return 1;
            }
            printf("Result of subtraction:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    printf("%d ", matrix1[i][j] - matrix2[i][j]);
                }
                printf("\n");
            }
            break;

        case OP_MULTIPLY:
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            printf("Result of multiplication:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    int sum = 0;
                    for (int k = 0; k < cols1; k++) {
                        sum += matrix1[i][k] * matrix2[k][j];
                    }
                    printf("%d ", sum);
                }
                printf("\n");
            }
            break;

        case OP_TRANSPOSE:
            printf("Transpose of first matrix:\n");
            for (int j = 0; j < cols1; j++) {
                for (int i = 0; i < rows1; i++) {
                    printf("%d ", matrix1[i][j]);
                }
                printf("\n");
            }
            break;

        case OP_INVALID:
            printf("Invalid operation selected\n");
            return 1;
    }

    return 0;
}