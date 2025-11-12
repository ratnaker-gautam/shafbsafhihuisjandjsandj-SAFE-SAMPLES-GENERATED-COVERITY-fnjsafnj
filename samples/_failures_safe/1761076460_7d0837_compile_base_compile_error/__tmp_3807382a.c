//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int op_choice;
    
    printf("Enter matrix 1 dimensions (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
        printf("Dimensions must be between 1 and 10\n");
        return 1;
    }
    
    int matrix1[10][10];
    printf("Enter matrix 1 elements:\n");
    read_matrix(rows1, cols1, matrix1);
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose, 4=determinant): ");
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid operation\n");
        return 1;
    }
    if (op_choice < 0 || op_choice > 4) {
        printf("Operation must be 0-4\n");
        return 1;
    }
    
    enum operation op = op_choice;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY: {
            printf("Enter matrix 2 dimensions (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
                printf("Dimensions must be between 1 and 10\n");
                return 1;
            }
            
            int matrix2[10][10];
            printf("Enter matrix 2 elements:\n");
            read_matrix(rows2, cols2, matrix2);
            
            if (op == OP_ADD || op == OP_SUBTRACT) {
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Matrix dimensions must match for addition/subtraction\n");
                    return 1;
                }
                int result[10][10];
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
            } else {
                if (cols1 != rows2) {
                    printf("Invalid dimensions for multiplication\n");
                    return 1;
                }
                int result[10][10] = {0};
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols2; j++) {
                        for (int k = 0; k < cols1; k++) {
                            result[i][j] += matrix1[i][k] * matrix2[k][j];
                        }
                    }
                }
                printf("Result:\n");
                print_matrix(rows1, cols2, result);
            }
            break;
        }
        
        case OP_TRANSPOSE: {
            int result[10][10];
            for (int i = 0; i < cols1; i++) {
                for (int j = 0; j < rows1; j++) {
                    result[i][j] = matrix1[j][i];
                }
            }
            printf("Result:\n");
            print_matrix(cols1, rows1, result);
            break;
        }
        
        case OP_DETERMINANT: {
            if (rows1 != cols1) {
                printf("Matrix must be square for determinant\n");
                return 1;
            }
            if (rows1 == 1) {
                printf("Determinant: %d\n", matrix1[0][0]);
            } else if (rows1 == 2) {
                int det = matrix1[0][0] * matrix1[1][1] - matrix1[0][1] * matrix1[1][0];
                printf("Determinant: %d\n", det);
            } else if (rows1 == 3) {
                int