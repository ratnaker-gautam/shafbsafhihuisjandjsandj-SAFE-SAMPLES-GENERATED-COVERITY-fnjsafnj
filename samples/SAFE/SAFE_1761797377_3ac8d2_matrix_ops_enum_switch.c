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
    
    printf("Enter matrix 2 dimensions (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("0: Add\n1: Subtract\n2: Multiply\n3: Transpose\n");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        printf("Invalid operation\n");
        return 1;
    }
    op = (enum Operation)choice;
    
    int mat1[rows1][cols1];
    int mat2[rows2][cols2];
    
    printf("Enter matrix 1 elements:\n");
    if (!read_matrix(rows1, cols1, mat1)) {
        printf("Invalid matrix elements\n");
        return 1;
    }
    
    if (op != OP_TRANSPOSE) {
        printf("Enter matrix 2 elements:\n");
        if (!read_matrix(rows2, cols2, mat2)) {
            printf("Invalid matrix elements\n");
            return 1;
        }
    }
    
    switch (op) {
        case OP_ADD:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            printf("Result:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    printf("%d ", mat1[i][j] + mat2[i][j]);
                }
                printf("\n");
            }
            break;
            
        case OP_SUBTRACT:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for subtraction\n");
                return 1;
            }
            printf("Result:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    printf("%d ", mat1[i][j] - mat2[i][j]);
                }
                printf("\n");
            }
            break;
            
        case OP_MULTIPLY:
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            printf("Result:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    int sum = 0;
                    for (int k = 0; k < cols1; k++) {
                        sum += mat1[i][k] * mat2[k][j];
                    }
                    printf("%d ", sum);
                }
                printf("\n");
            }
            break;
            
        case OP_TRANSPOSE:
            printf("Transpose of matrix 1:\n");
            for (int j = 0; j < cols1; j++) {
                for (int i = 0; i < rows1; i++) {
                    printf("%d ", mat1[i][j]);
                }
                printf("\n");
            }
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}