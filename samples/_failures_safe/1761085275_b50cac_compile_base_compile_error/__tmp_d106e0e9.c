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
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
        printf("Invalid dimensions\n");
        return 1;
    }

    int matrix1[10][10];
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
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
        printf("Invalid dimensions\n");
        return 1;
    }

    int matrix2[10][10];
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("Select operation:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose first matrix\n");
    int op_choice;
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid input\n");
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

    int result[10][10];
    int res_rows, res_cols;

    switch (op) {
        case OP_ADD:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            res_rows = rows1;
            res_cols = cols1;
            for (int i = 0; i < res_rows; i++) {
                for (int j = 0; j < res_cols; j++) {
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            printf("Result of addition:\n");
            print_matrix(res_rows, res_cols, result);
            break;

        case OP_SUBTRACT:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for subtraction\n");
                return 1;
            }
            res_rows = rows1;
            res_cols = cols1;
            for (int i = 0; i < res_rows; i++) {
                for (int j = 0; j < res_cols; j++) {
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
            printf("Result of subtraction:\n");
            print_matrix(res_rows, res_cols, result);
            break;

        case OP_MULTIPLY:
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            res_rows = rows1;
            res_cols = cols2;
            for (int i = 0; i < res_rows; i++) {
                for (int j = 0; j < res_cols; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                }
            }
            printf("Result of multiplication:\n");
            print_matrix(res_rows, res_cols, result);
            break;

        case OP_TRANSPOSE:
            res_rows = cols1;
            res_cols = rows1;
            for (int i = 0; i < res_rows; i++) {
                for (int j = 0; j < res_cols; j++) {
                    result[i][j] = matrix1[j][