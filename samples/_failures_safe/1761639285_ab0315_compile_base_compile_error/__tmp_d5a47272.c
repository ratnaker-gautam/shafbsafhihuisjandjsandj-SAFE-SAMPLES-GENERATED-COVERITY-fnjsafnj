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

void input_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

int determinant_2x2(int matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

int main(void) {
    enum Operation op;
    int choice;
    printf("Select operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose, 4=Determinant): ");
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
            printf("Enter matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            int mat1[rows][cols];
            int mat2[rows][cols];
            int result[rows][cols];
            printf("Enter first matrix:\n");
            input_matrix(rows, cols, mat1);
            printf("Enter second matrix:\n");
            input_matrix(rows, cols, mat2);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (op == OP_ADD) {
                        result[i][j] = mat1[i][j] + mat2[i][j];
                    } else {
                        result[i][j] = mat1[i][j] - mat2[i][j];
                    }
                }
            }
            printf("Result:\n");
            print_matrix(rows, cols, result);
            break;
        }
        case OP_MULTIPLY: {
            int rows1, cols1, rows2, cols2;
            printf("Enter first matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            printf("Enter second matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (cols1 != rows2) {
                printf("Dimension mismatch\n");
                return 1;
            }
            int mat1[rows1][cols1];
            int mat2[rows2][cols2];
            int result[rows1][cols2];
            printf("Enter first matrix:\n");
            input_matrix(rows1, cols1, mat1);
            printf("Enter second matrix:\n");
            input_matrix(rows2, cols2, mat2);
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += mat1[i][k] * mat2[k][j];
                    }
                }
            }
            printf("Result:\n");
            print_matrix(rows1, cols2, result);
            break;
        }
        case OP_TRANSPOSE: {
            int rows, cols;
            printf("Enter matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            int matrix[rows][cols];
            int transpose[cols][rows];
            printf("Enter matrix:\n");
            input_matrix(rows, cols, matrix);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    transpose[j][i] = matrix[i][j];
                }
            }
            printf("Transpose:\n");
            print_matrix(cols, rows, transpose);
            break;
        }
        case OP_DETERMINANT: {
            int size;
            printf("Enter matrix size (2 for 2x2): ");
            if (scanf("%d", &size) !=