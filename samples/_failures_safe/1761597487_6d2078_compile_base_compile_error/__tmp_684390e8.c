//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

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
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int validate_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > 10 || cols > 10) {
        printf("Invalid dimensions: must be between 1 and 10\n");
        return 0;
    }
    return 1;
}

int determinant(int n, int matrix[n][n]) {
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    int det = 0;
    int sign = 1;
    int submatrix[10][10];
    
    for (int k = 0; k < n; k++) {
        int sub_i = 0;
        for (int i = 1; i < n; i++) {
            int sub_j = 0;
            for (int j = 0; j < n; j++) {
                if (j == k) continue;
                submatrix[sub_i][sub_j] = matrix[i][j];
                sub_j++;
            }
            sub_i++;
        }
        det += sign * matrix[0][k] * determinant(n - 1, submatrix);
        sign = -sign;
    }
    return det;
}

int main(void) {
    enum operation op;
    int rows1, cols1, rows2, cols2;
    
    printf("Select operation:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("4: Determinant\n");
    printf("Enter operation number: ");
    
    if (scanf("%d", (int*)&op) != 1) {
        printf("Invalid operation\n");
        return EXIT_FAILURE;
    }
    
    if (op < OP_ADD || op > OP_DETERMINANT) {
        printf("Invalid operation\n");
        return EXIT_FAILURE;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter rows and columns for both matrices: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            if (!validate_dimensions(rows1, cols1)) {
                return EXIT_FAILURE;
            }
            rows2 = rows1;
            cols2 = cols1;
            break;
            
        case OP_MULTIPLY:
            printf("Enter rows and columns for first matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            printf("Enter rows and columns for second matrix: ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                printf("Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            if (!validate_dimensions(rows1, cols1) || !validate_dimensions(rows2, cols2)) {
                return EXIT_FAILURE;
            }
            if (cols1 != rows2) {
                printf("Matrix dimensions incompatible for multiplication\n");
                return EXIT_FAILURE;
            }
            break;
            
        case OP_TRANSPOSE:
        case OP_DETERMINANT:
            printf("Enter rows and columns for matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                printf("Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            if (!validate_dimensions(rows1, cols1)) {
                return EXIT_FAILURE;
            }
            if (op == OP_DETERMINANT && rows1 != cols1) {
                printf("Determinant requires square matrix\n");
                return EXIT_FAILURE;
            }
            break;
    }
    
    int matrix1[10][10] = {0};
    int matrix2[10][10] = {0};
    int result[10][10] = {0};
    
    printf("Enter first matrix:\n");
    read_matrix(rows1, cols1, matrix1);
    
    if (op == OP_ADD || op == OP_SUBTRACT || op == OP_MULTIPLY) {
        printf("Enter second matrix:\n