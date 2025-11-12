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

int add_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return 1;
}

int subtract_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return 1;
}

int multiply_matrices(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
    if (cols1 != rows2) {
        return 0;
    }
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return 1;
}

int transpose_matrix(int rows, int cols, int matrix[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return 1;
}

enum Operation get_operation() {
    char input[16];
    if (scanf("%15s", input) != 1) {
        return OP_INVALID;
    }
    if (strcmp(input, "add") == 0) return OP_ADD;
    if (strcmp(input, "subtract") == 0) return OP_SUBTRACT;
    if (strcmp(input, "multiply") == 0) return OP_MULTIPLY;
    if (strcmp(input, "transpose") == 0) return OP_TRANSPOSE;
    return OP_INVALID;
}

int main() {
    enum Operation op = get_operation();
    if (op == OP_INVALID) {
        printf("Invalid operation\n");
        return 1;
    }

    int rows1, cols1;
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    int matrix1[100][100];
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix data\n");
        return 1;
    }

    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows2, cols2;
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
                printf("Invalid second matrix dimensions\n");
                return 1;
            }
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            int matrix2[100][100];
            if (!read_matrix(rows2, cols2, matrix2)) {
                printf("Invalid second matrix data\n");
                return 1;
            }
            int result[100][100];
            if (op == OP_ADD) {
                add_matrices(rows1, cols1, matrix1, matrix2, result);
            } else {
                subtract_matrices(rows1, cols1, matrix1, matrix2, result);
            }
            print_matrix(rows1, cols1, result);
            break;
        }
        case OP_MULTIPLY: {
            int rows2, cols2;
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 >