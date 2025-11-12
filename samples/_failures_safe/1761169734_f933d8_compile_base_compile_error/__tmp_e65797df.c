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
                exit(1);
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
    int temp[10][10];
    
    for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) continue;
                temp[subi][subj] = matrix[i][j];
                subj++;
            }
            subi++;
        }
        int sign = (x % 2 == 0) ? 1 : -1;
        det += sign * matrix[0][x] * determinant(n - 1, temp);
    }
    return det;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    enum operation op;
    
    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=transpose, 4=determinant): ");
    if (scanf("%d", (int*)&op) != 1 || op < 0 || op > 4) {
        printf("Invalid operation\n");
        return 1;
    }
    
    printf("Enter rows for matrix 1: ");
    if (scanf("%d", &rows1) != 1 || rows1 <= 0 || rows1 > 10) {
        printf("Invalid rows\n");
        return 1;
    }
    
    printf("Enter columns for matrix 1: ");
    if (scanf("%d", &cols1) != 1 || cols1 <= 0 || cols1 > 10) {
        printf("Invalid columns\n");
        return 1;
    }
    
    int matrix1[10][10];
    printf("Enter elements for matrix 1:\n");
    read_matrix(rows1, cols1, matrix1);
    
    int matrix2[10][10] = {0};
    int result[10][10] = {0};
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY:
            printf("Enter rows for matrix 2: ");
            if (scanf("%d", &rows2) != 1 || rows2 <= 0 || rows2 > 10) {
                printf("Invalid rows\n");
                return 1;
            }
            printf("Enter columns for matrix 2: ");
            if (scanf("%d", &cols2) != 1 || cols2 <= 0 || cols2 > 10) {
                printf("Invalid columns\n");
                return 1;
            }
            printf("Enter elements for matrix 2:\n");
            read_matrix(rows2, cols2, matrix2);
            break;
        default:
            rows2 = 0;
            cols2 = 0;
            break;
    }
    
    switch (op) {
        case OP_ADD:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            printf("Result of addition:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case OP_SUBTRACT:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for subtraction\n");
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
            printf("Result of subtraction:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case OP_MULTIPLY:
            if (cols1 != rows2) {
                printf