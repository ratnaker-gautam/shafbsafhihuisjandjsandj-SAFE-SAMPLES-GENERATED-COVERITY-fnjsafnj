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

void add_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void subtract_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void multiply_matrices(int a_rows, int a_cols, int b_cols, int a[a_rows][a_cols], int b[a_cols][b_cols], int result[a_rows][b_cols]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int determinant_2x2(int matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

int main(void) {
    enum Operation op;
    int choice;
    
    printf("Matrix Operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("4: Determinant (2x2 only)\n");
    printf("Select operation: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 4) {
        printf("Invalid operation\n");
        return 1;
    }
    
    op = (enum Operation)choice;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows, cols;
            printf("Enter rows and columns: ");
            if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            
            int matrix1[rows][cols];
            int matrix2[rows][cols];
            int result[rows][cols];
            
            printf("Enter first matrix:\n");
            if (!read_matrix(rows, cols, matrix1)) {
                printf("Invalid input\n");
                return 1;
            }
            
            printf("Enter second matrix:\n");
            if (!read_matrix(rows, cols, matrix2)) {
                printf("Invalid input\n");
                return 1;
            }
            
            if (op == OP_ADD) {
                add_matrices(rows, cols, matrix1, matrix2, result);
                printf("Result:\n");
                print_matrix(rows, cols, result);
            } else {
                subtract_matrices(rows, cols, matrix1, matrix2, result);
                printf("Result:\n");
                print_matrix(rows, cols, result);
            }
            break;
        }
        
        case OP_MULTIPLY: {
            int rows1, cols1, cols2;
            printf("Enter first matrix rows and columns: ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            
            printf("Enter second matrix columns: ");
            if (scanf("%d", &cols2) != 1 || cols2 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            
            int matrix1[rows1][cols1];
            int matrix2[cols