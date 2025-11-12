//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

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

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

int determinant(int n, int matrix[n][n]) {
    if (n == 1) return matrix[0][0];
    if (n == 2) {
        int det = safe_multiply(matrix[0][0], matrix[1][1]);
        int sub = safe_multiply(matrix[0][1], matrix[1][0]);
        return safe_add(det, -sub);
    }
    
    int det = 0;
    int temp[10][10];
    int sign = 1;
    
    for (int k = 0; k < n; k++) {
        int i_sub = 0;
        for (int i = 1; i < n; i++) {
            int j_sub = 0;
            for (int j = 0; j < n; j++) {
                if (j == k) continue;
                temp[i_sub][j_sub] = matrix[i][j];
                j_sub++;
            }
            i_sub++;
        }
        int minor_det = determinant(n - 1, temp);
        int term = safe_multiply(sign, safe_multiply(matrix[0][k], minor_det));
        det = safe_add(det, term);
        sign = -sign;
    }
    return det;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 100 || cols1 > 100) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    int matrix1[100][100];
    printf("Enter %d elements for first matrix:\n", rows1 * cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    int op_choice;
    printf("Select operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose, 4=Determinant): ");
    if (scanf("%d", &op_choice) != 1 || op_choice < 0 || op_choice > 4) {
        printf("Invalid operation\n");
        return 1;
    }

    enum Operation op = op_choice;
    int result[100][100];

    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("Enter dimensions of second matrix (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > 100 || cols2 > 100) {
                printf("Invalid matrix dimensions\n");
                return 1;
            }
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                return 1;
            }
            int matrix2[100][100];
            printf("Enter %d elements for second matrix:\n", rows2 * cols2);
            for (int i = 0; i < rows2; i++) {
                for (int j = 0; j < cols2; j++) {
                    if (scanf("%d", &matrix2[i][j]) != 1) {
                        printf("Invalid input\n");
                        return 1;
                    }
                }
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (op == OP_ADD) {
                        result[i][j] = safe_add(matrix1[i][j], matrix2[i][j]);
                    } else {
                        result[i][j] = safe_add(matrix1[i][j], -matrix2[i][j]);
                    }
                }
            }