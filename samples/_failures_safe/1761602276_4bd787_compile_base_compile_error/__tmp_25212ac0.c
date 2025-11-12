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
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Invalid input\n");
                exit(EXIT_FAILURE);
            }
        }
    }
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
    int submatrix[n-1][n-1];
    
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
        det += sign * matrix[0][k] * determinant(n-1, submatrix);
        sign = -sign;
    }
    return det;
}

int main(void) {
    enum operation op;
    int rows1, cols1, rows2, cols2;
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose, 4=determinant): ");
    if (scanf("%d", (int*)&op) != 1) {
        fprintf(stderr, "Invalid operation\n");
        return EXIT_FAILURE;
    }
    
    if (op < OP_ADD || op > OP_DETERMINANT) {
        fprintf(stderr, "Invalid operation\n");
        return EXIT_FAILURE;
    }
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter rows and columns for both matrices: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                fprintf(stderr, "Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                fprintf(stderr, "Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            rows2 = rows1;
            cols2 = cols1;
            break;
            
        case OP_MULTIPLY:
            printf("Enter rows and columns for first matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                fprintf(stderr, "Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                fprintf(stderr, "Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            printf("Enter rows and columns for second matrix: ");
            if (scanf("%d %d", &rows2, &cols2) != 2) {
                fprintf(stderr, "Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            if (rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
                fprintf(stderr, "Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            if (cols1 != rows2) {
                fprintf(stderr, "Incompatible dimensions for multiplication\n");
                return EXIT_FAILURE;
            }
            break;
            
        case OP_TRANSPOSE:
        case OP_DETERMINANT:
            printf("Enter rows and columns for matrix: ");
            if (scanf("%d %d", &rows1, &cols1) != 2) {
                fprintf(stderr, "Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            if (rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
                fprintf(stderr, "Invalid dimensions\n");
                return EXIT_FAILURE;
            }
            rows2 = rows1;
            cols2 = cols1;
            break;
    }
    
    int matrix1[rows1][cols1];
    int matrix2[rows2][cols2];
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter first matrix elements:\n");
            read_matrix(rows1, cols1, matrix1);
            printf("Enter second matrix elements:\n");
            read_matrix(rows2, cols2, matrix2);
            printf