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
    
    printf("Enter operation (add/subtract/multiply/transpose): ");
    enum Operation op = get_operation();
    if (op == OP_INVALID) {
        printf("Invalid operation\n");
        return 1;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter dimensions for both matrices (rows cols): ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            rows2 = rows1;
            cols2 = cols1;
            break;
            
        case OP_MULTIPLY:
            printf("Enter dimensions for first matrix (rows cols): ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            printf("Enter dimensions for second matrix (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (cols1 != rows2) {
                printf("Dimension mismatch for multiplication\n");
                return 1;
            }
            break;
            
        case OP_TRANSPOSE:
            printf("Enter matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            rows2 = cols1;
            cols2 = rows1;
            break;
            
        default:
            return 1;
    }
    
    int matrix1[rows1][cols1];
    int matrix2[rows2][cols2];
    int result[rows1][cols2];
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter first matrix (%dx%d):\n", rows1, cols1);
            if (!read_matrix(rows1, cols1, matrix1)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            printf("Enter second matrix (%dx%d):\n", rows2, cols2);
            if (!read_matrix(rows2, cols2, matrix2)) {
                printf("Invalid matrix input\n");
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
            printf("Result:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case OP_MULTIPLY:
            printf("Enter first matrix (%dx%d):\n", rows1, cols1);
            if (!read_matrix(rows1, cols1, matrix1)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            printf("Enter second matrix (%dx%d):\n", rows2, cols2);
            if (!read_matrix(rows2, cols2, matrix2)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k