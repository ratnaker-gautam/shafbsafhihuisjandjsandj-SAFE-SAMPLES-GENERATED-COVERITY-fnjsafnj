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
    
    printf("Enter matrix 1 elements:\n");
    int matrix1[rows1][cols1];
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    printf("Matrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("Select operation:\n");
    printf("0: Add\n1: Subtract\n2: Multiply\n3: Transpose\n");
    if (scanf("%d", &choice) != 1 || choice < 0 || choice > 3) {
        printf("Invalid operation\n");
        return 1;
    }
    op = (enum Operation)choice;
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        printf("Enter matrix 2 dimensions (rows cols): ");
        if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
            printf("Invalid dimensions\n");
            return 1;
        }
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Dimension mismatch for addition/subtraction\n");
            return 1;
        }
        
        printf("Enter matrix 2 elements:\n");
        int matrix2[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        printf("Matrix 2:\n");
        print_matrix(rows2, cols2, matrix2);
        
        printf("Result:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                if (op == OP_ADD) {
                    printf("%d ", matrix1[i][j] + matrix2[i][j]);
                } else {
                    printf("%d ", matrix1[i][j] - matrix2[i][j]);
                }
            }
            printf("\n");
        }
    } else if (op == OP_MULTIPLY) {
        printf("Enter matrix 2 dimensions (rows cols): ");
        if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
            printf("Invalid dimensions\n");
            return 1;
        }
        if (cols1 != rows2) {
            printf("Dimension mismatch for multiplication\n");
            return 1;
        }
        
        printf("Enter matrix 2 elements:\n");
        int matrix2m[rows2][cols2];
        if (!read_matrix(rows2, cols2, matrix2m)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        printf("Matrix 2:\n");
        print_matrix(rows2, cols2, matrix2m);
        
        printf("Result:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                int sum = 0;
                for (int k = 0; k < cols1; k++) {
                    sum += matrix1[i][k] * matrix2m[k][j];
                }
                printf("%d ", sum);
            }
            printf("\n");
        }
    } else if (op == OP_TRANSPOSE) {
        printf("Result:\n");
        for (int j = 0; j < cols1; j++) {
            for (int i = 0; i < rows1; i++) {
                printf("%d ", matrix1[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("Invalid operation\n");
        return 1;
    }
    
    return 0;
}