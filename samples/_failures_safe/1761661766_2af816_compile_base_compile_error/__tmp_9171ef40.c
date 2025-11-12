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
    int choice;
    
    printf("Enter matrix 1 dimensions (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[rows1][cols1];
    printf("Enter %d x %d matrix 1:\n", rows1, cols1);
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    printf("Select operation:\n");
    printf("0: Add\n1: Subtract\n2: Multiply\n3: Transpose\n");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid operation choice\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        op = OP_INVALID;
    } else {
        op = (enum Operation)choice;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter matrix 2 dimensions (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Dimension mismatch for addition/subtraction\n");
                return 1;
            }
            break;
            
        case OP_MULTIPLY:
            printf("Enter matrix 2 dimensions (rows cols): ");
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
            rows2 = cols1;
            cols2 = rows1;
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    if (op != OP_TRANSPOSE) {
        int matrix2[rows2][cols2];
        printf("Enter %d x %d matrix 2:\n", rows2, cols2);
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        switch (op) {
            case OP_ADD: {
                int result[rows1][cols1];
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                        result[i][j] = matrix1[i][j] + matrix2[i][j];
                    }
                }
                printf("Result:\n");
                print_matrix(rows1, cols1, result);
                break;
            }
            
            case OP_SUBTRACT: {
                int result[rows1][cols1];
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                        result[i][j] = matrix1[i][j] - matrix2[i][j];
                    }
                }
                printf("Result:\n");
                print_matrix(rows1, cols1, result);
                break;
            }
            
            case OP_MULTIPLY: {
                int result[rows1][cols2];
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols2; j++) {
                        result[i][j] = 0;
                        for (int k = 0; k < cols1; k++) {
                            result[i][j] += matrix1[i][k] * matrix2[k][j];
                        }
                    }
                }
                printf("Result:\n");
                print_matrix(rows1, cols2, result);
                break;
            }
            
            default:
                break;
        }
    } else {
        int result[cols1][rows1];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                result[j][i] = matrix