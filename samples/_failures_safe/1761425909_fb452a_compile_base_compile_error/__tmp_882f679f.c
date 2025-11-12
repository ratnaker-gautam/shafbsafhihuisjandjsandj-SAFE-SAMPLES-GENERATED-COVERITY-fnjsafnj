//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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

void multiply_matrices(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
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
    enum operation op;
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
    op = (enum operation)choice;

    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows, cols;
            printf("Enter matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            int a[rows][cols], b[rows][cols], result[rows][cols];
            printf("Enter first matrix:\n");
            if (!read_matrix(rows, cols, a)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            printf("Enter second matrix:\n");
            if (!read_matrix(rows, cols, b)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            if (op == OP_ADD) {
                add_matrices(rows, cols, a, b, result);
            } else {
                subtract_matrices(rows, cols, a, b, result);
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
                printf("Dimension mismatch for multiplication\n");
                return 1;
            }
            int a[rows1][cols1], b[rows2][cols2], result[rows1][cols2];
            printf("Enter first matrix:\n");
            if (!