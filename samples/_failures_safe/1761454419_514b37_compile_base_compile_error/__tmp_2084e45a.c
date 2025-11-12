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

enum Operation get_operation(const char* input) {
    if (strcmp(input, "add") == 0) return OP_ADD;
    if (strcmp(input, "subtract") == 0) return OP_SUBTRACT;
    if (strcmp(input, "multiply") == 0) return OP_MULTIPLY;
    if (strcmp(input, "transpose") == 0) return OP_TRANSPOSE;
    return OP_INVALID;
}

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

void add_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void subtract_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main(void) {
    char op_str[16];
    printf("Enter operation (add/subtract/multiply/transpose): ");
    if (scanf("%15s", op_str) != 1) {
        fprintf(stderr, "Error reading operation\n");
        return 1;
    }

    enum Operation op = get_operation(op_str);
    if (op == OP_INVALID) {
        fprintf(stderr, "Invalid operation\n");
        return 1;
    }

    int rows1, cols1;
    printf("Enter rows and columns for first matrix: ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }

    int matrix1[100][100];
    printf("Enter first matrix elements:\n");
    if (!read_matrix(rows1, cols1, matrix1)) {
        fprintf(stderr, "Error reading matrix elements\n");
        return 1;
    }

    int rows2, cols2;
    int matrix2[100][100];
    if (op != OP_TRANSPOSE) {
        printf("Enter rows and columns for second matrix: ");
        if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
            fprintf(stderr, "Invalid matrix dimensions\n");
            return 1;
        }
        printf("Enter second matrix elements:\n");
        if (!read_matrix(rows2, cols2, matrix2)) {
            fprintf(stderr, "Error reading matrix elements\n");
            return 1;
        }
    }

    int result[100][100];
    int result_rows, result_cols;

    switch (op) {
        case OP_ADD:
            if (rows1 != rows2 || cols1 != cols2) {
                fprintf(stderr, "Matrix dimensions must match for addition\n");
                return 1;
            }
            add_matrices(rows1, cols1, matrix1, matrix2, result);
            result_rows = rows1;
            result_cols = cols1