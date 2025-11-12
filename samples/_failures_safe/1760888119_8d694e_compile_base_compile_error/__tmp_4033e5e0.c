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

enum Operation parse_operation(const char* input) {
    if (strcmp(input, "add") == 0) return OP_ADD;
    if (strcmp(input, "subtract") == 0) return OP_SUBTRACT;
    if (strcmp(input, "multiply") == 0) return OP_MULTIPLY;
    if (strcmp(input, "transpose") == 0) return OP_TRANSPOSE;
    return OP_INVALID;
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

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrix_add(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void matrix_subtract(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void matrix_multiply(int a_rows, int a_cols, int b_rows, int b_cols, int a[a_rows][a_cols], int b[b_rows][b_cols], int result[a_rows][b_cols]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main(void) {
    char op_str[16];
    if (scanf("%15s", op_str) != 1) {
        fprintf(stderr, "Error reading operation\n");
        return 1;
    }

    enum Operation op = parse_operation(op_str);
    if (op == OP_INVALID) {
        fprintf(stderr, "Invalid operation\n");
        return 1;
    }

    int rows1, cols1;
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Error reading matrix dimensions\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > 100 || cols1 <= 0 || cols1 > 100) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }

    int matrix1[100][100];
    if (!read_matrix(rows1, cols1, matrix1)) {
        fprintf(stderr, "Error reading matrix 1\n");
        return 1;
    }

    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows2, cols2;
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                fprintf(stderr, "Error reading matrix 2 dimensions\n");
                return 1;
            }
            if (rows2 != rows1 || cols2 != cols1) {
                fprintf(stderr, "Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            int matrix2[100][100];
            if (!read_matrix(rows2, cols2, matrix2)) {
                fprintf(stderr, "Error reading matrix 2\n");
                return 1;
            }
            int result[100][100];
            if (op == OP_ADD) {
                matrix_add(rows1, cols1, matrix1, matrix2, result);
            } else {
                matrix_subtract(rows1, cols1, matrix1, matrix2, result);
            }
            print_matrix(rows1, cols1, result);
            break;
        }
        case OP_MULTIPLY: {
            int rows2, cols2;
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                fprintf(stderr, "