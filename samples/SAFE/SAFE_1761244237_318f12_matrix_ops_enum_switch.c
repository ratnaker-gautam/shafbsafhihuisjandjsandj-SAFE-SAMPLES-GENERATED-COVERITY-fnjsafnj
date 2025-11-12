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
    char op_str[16];
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
        fprintf(stderr, "Error reading matrix data\n");
        return 1;
    }

    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY: {
            int rows2, cols2;
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                fprintf(stderr, "Error reading second matrix dimensions\n");
                return 1;
            }
            if (rows2 <= 0 || rows2 > 100 || cols2 <= 0 || cols2 > 100) {
                fprintf(stderr, "Invalid second matrix dimensions\n");
                return 1;
            }

            int matrix2[100][100];
            if (!read_matrix(rows2, cols2, matrix2)) {
                fprintf(stderr, "Error reading second matrix data\n");
                return 1;
            }

            int result[100][100];
            if (op == OP_ADD || op == OP_SUBTRACT) {
                if (rows1 != rows2 || cols1 != cols2) {
                    fprintf(stderr, "Matrix dimensions must match for addition/subtraction\n");
                    return 1;
                }
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
            } else if (op == OP_MULTIPLY) {
                if (cols1 != rows2) {
                    fprintf(stderr, "Invalid dimensions for multiplication\n");
                    return 1;
                }
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
            break;
        }
        case OP_TRANSPOSE: {
            int result[100][100];
            for (int i = 0; i < cols1; i++) {
                for (int j = 0; j < rows1; j++) {
                    result[i][j] = matrix1[j][i];
                }
            }
            print_matrix(cols1, rows1, result);
            break;
        }
        default:
            fprintf(stderr, "Unsupported operation\n");
            return 1;
    }

    return 0;
}