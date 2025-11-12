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

int main(void) {
    int rows1, cols1, rows2, cols2;
    enum Operation op;
    
    printf("Enter matrix 1 dimensions (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    if (rows1 > 100 || cols1 > 100) {
        printf("Matrix too large\n");
        return 1;
    }
    
    printf("Enter matrix 1 elements:\n");
    int matrix1[100][100];
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix elements\n");
        return 1;
    }
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose): ");
    int op_input;
    if (scanf("%d", &op_input) != 1) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (op_input < 0 || op_input > 3) {
        printf("Invalid operation\n");
        return 1;
    }
    op = (enum Operation)op_input;
    
    int result_rows, result_cols;
    int matrix2[100][100];
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter matrix 2 dimensions (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            if (rows2 > 100 || cols2 > 100) {
                printf("Matrix too large\n");
                return 1;
            }
            printf("Enter matrix 2 elements:\n");
            if (!read_matrix(rows2, cols2, matrix2)) {
                printf("Invalid matrix elements\n");
                return 1;
            }
            result_rows = rows1;
            result_cols = cols1;
            break;
            
        case OP_MULTIPLY:
            printf("Enter matrix 2 dimensions (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (cols1 != rows2) {
                printf("Matrix dimensions incompatible for multiplication\n");
                return 1;
            }
            if (rows2 > 100 || cols2 > 100) {
                printf("Matrix too large\n");
                return 1;
            }
            printf("Enter matrix 2 elements:\n");
            if (!read_matrix(rows2, cols2, matrix2)) {
                printf("Invalid matrix elements\n");
                return 1;
            }
            result_rows = rows1;
            result_cols = cols2;
            break;
            
        case OP_TRANSPOSE:
            result_rows = cols1;
            result_cols = rows1;
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    if (result_rows > 100 || result_cols > 100) {
        printf("Result matrix too large\n");
        return 1;
    }
    
    int result[100][100];
    
    switch (op) {
        case OP_ADD:
            for (int i = 0; i < result_rows; i++) {
                for (int j = 0; j < result_cols; j++) {
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            break;
            
        case OP_SUBTRACT:
            for (int i = 0; i < result_rows; i++) {
                for (int j = 0; j < result_cols; j++) {
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
            break;
            
        case OP_MULTIPLY:
            for (int i = 0; i < result_rows; i++) {
                for (int j = 0; j < result_cols; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1