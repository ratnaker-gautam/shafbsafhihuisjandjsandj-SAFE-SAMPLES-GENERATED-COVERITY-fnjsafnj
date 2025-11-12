//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
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
            printf("%d", matrix[i][j]);
            if (j < cols - 1) printf(" ");
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
    int rows1, cols1, rows2, cols2;
    
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Error reading matrix dimensions\n");
        return 1;
    }
    
    if (rows1 <= 0 || rows1 > 100 || cols1 <= 0 || cols1 > 100) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    int matrix1[rows1][cols1];
    if (!read_matrix(rows1, cols1, matrix1)) {
        fprintf(stderr, "Error reading matrix data\n");
        return 1;
    }
    
    enum Operation op = get_operation();
    if (op == OP_INVALID) {
        fprintf(stderr, "Invalid operation\n");
        return 1;
    }
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        if (scanf("%d %d", &rows2, &cols2) != 2) {
            fprintf(stderr, "Error reading second matrix dimensions\n");
            return 1;
        }
        if (rows2 != rows1 || cols2 != cols1) {
            fprintf(stderr, "Matrix dimensions must match for this operation\n");
            return 1;
        }
        if (rows2 <= 0 || rows2 > 100 || cols2 <= 0 || cols2 > 100) {
            fprintf(stderr, "Invalid second matrix dimensions\n");
            return 1;
        }
        int matrix2[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2)) {
            fprintf(stderr, "Error reading second matrix data\n");
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
    } else if (op == OP_MULTIPLY) {
        if (scanf("%d %d", &rows2, &cols2) != 2) {
            fprintf(stderr, "Error reading second matrix dimensions\n");
            return 1;
        }
        if (cols1 != rows2) {
            fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
            return 1;
        }
        if (rows2 <= 0 || rows2 > 100 || cols2 <= 0 || cols2 > 100) {
            fprintf(stderr, "Invalid second matrix dimensions\n");
            return 1;
        }
        int matrix2_mul[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2_mul)) {
            fprintf(stderr, "Error reading second matrix data\n");
            return 1;
        }
        int result_mul[rows1][cols2];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                result_mul[i][j] = 0;
                for (int k = 0; k < cols1; k++) {
                    result_mul[i][j] += matrix1[i][k] * matrix2_mul[k][j];
                }
            }
        }
        print_matrix(rows1, cols2, result_mul);
    } else if (op == OP_TRANSPOSE) {
        int result_trans[cols1][rows1];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols