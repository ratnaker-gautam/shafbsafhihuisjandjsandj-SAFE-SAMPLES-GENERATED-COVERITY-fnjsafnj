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

int multiply_matrices(int a_rows, int a_cols, int b_rows, int b_cols, int a[a_rows][a_cols], int b[b_rows][b_cols], int result[a_rows][b_cols]) {
    if (a_cols != b_rows) {
        return 0;
    }
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
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

int main(void) {
    enum Operation op;
    int op_input;
    int rows1, cols1, rows2, cols2;
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose): ");
    if (scanf("%d", &op_input) != 1) {
        printf("Invalid operation input\n");
        return 1;
    }
    
    if (op_input < 0 || op_input > 3) {
        printf("Invalid operation\n");
        return 1;
    }
    op = (enum Operation)op_input;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter dimensions for both matrices (rows cols): ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid dimensions\n");
                return 1;
            }
            rows2 = rows1;
            cols2 = cols1;
            break;
            
        case OP_MULTIPLY:
            printf("Enter dimensions for first matrix (rows cols): ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid dimensions\n");
                return 1;
            }
            printf("Enter dimensions for second matrix (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
                printf("Invalid dimensions\n");
                return 1;
            }
            break;
            
        case OP_TRANSPOSE:
            printf("Enter matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                printf("Invalid dimensions\n");
                return 1;
            }
            rows2 = cols1;
            cols2 = rows1;
            break;