//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        int det = safe_multiply(matrix[0][0], matrix[1][1]);
        int sub = safe_multiply(matrix[0][1], matrix[1][0]);
        return safe_add(det, -sub);
    }
    
    int det = 0;
    int temp[10][10];
    
    for (int p = 0; p < n; p++) {
        int h = 0, k = 0;
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (j == p) continue;
                temp[h][k] = matrix[i][j];
                k++;
                if (k == n - 1) {
                    h++;
                    k = 0;
                }
            }
        }
        int sign = (p % 2 == 0) ? 1 : -1;
        int minor_det = determinant(n - 1, temp);
        int term = safe_multiply(sign, safe_multiply(matrix[0][p], minor_det));
        det = safe_add(det, term);
    }
    return det;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int op_choice;
    
    printf("Enter first matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > 10 || cols1 > 10) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[10][10];
    printf("Enter %d elements for first matrix:\n", rows1 * cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("4: Determinant\n");
    printf("Select operation: ");
    
    if (scanf("%d", &op_choice) != 1 || op_choice < 0 || op_choice > 4) {
        printf("Invalid operation\n");
        return 1;
    }
    
    enum Operation op = op_choice;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY:
            printf("Enter second matrix dimensions (rows cols): ");
            if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > 10 || cols2 > 10) {
                printf("Invalid dimensions\n");
                return 1;
            }
            
            int matrix2[10][10];
            printf("Enter %d elements for second matrix:\n", rows2 * cols2);
            for (int i = 0; i < rows2; i++) {
                for (int j = 0; j < cols2; j++) {
                    if (scanf("%d", &matrix2[i][j]) != 1) {
                        printf("Invalid input\n");
                        return 1;
                    }
                }
            }
            
            if (op == OP_ADD || op == OP_SUBTRACT) {
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Matrix dimensions must match for addition/subtraction\n");
                    return 1;
                }
                
                int result[10][10];
                for (int i = 0; i < rows1; i++) {
                    for (int j =