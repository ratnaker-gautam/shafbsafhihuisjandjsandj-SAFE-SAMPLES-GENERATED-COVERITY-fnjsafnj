//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_INVALID
};

enum Operation get_operation() {
    int choice;
    printf("Select operation:\n");
    printf("0: Add matrices\n");
    printf("1: Subtract matrices\n");
    printf("2: Multiply matrices\n");
    printf("3: Transpose matrix\n");
    printf("Enter choice (0-3): ");
    
    if (scanf("%d", &choice) != 1) {
        return OP_INVALID;
    }
    
    if (choice < 0 || choice > 3) {
        return OP_INVALID;
    }
    
    return (enum Operation)choice;
}

int read_matrix_size(const char* prompt) {
    int size;
    printf("%s", prompt);
    
    if (scanf("%d", &size) != 1) {
        return -1;
    }
    
    if (size <= 0 || size > 10) {
        return -1;
    }
    
    return size;
}

void read_matrix(int rows, int cols, int matrix[10][10]) {
    for (int i = 0; i < rows; i++) {
        printf("Enter row %d (%d values): ", i + 1, cols);
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                matrix[i][j] = 0;
            }
        }
    }
}

void print_matrix(int rows, int cols, int matrix[10][10]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrix_add(int rows, int cols, int a[10][10], int b[10][10], int result[10][10]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                result[i][j] = 0;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}

void matrix_subtract(int rows, int cols, int a[10][10], int b[10][10], int result[10][10]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] < INT_MIN + b[i][j]) ||
                (b[i][j] < 0 && a[i][j] > INT_MAX + b[i][j])) {
                result[i][j] = 0;
            } else {
                result[i][j] = a[i][j] - b[i][j];
            }
        }
    }
}

void matrix_multiply(int a_rows, int a_cols, int b_cols, int a[10][10], int b[10][10], int result[10][10]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    result[i][j] = 0;
                    break;
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    result[i][j] = 0;
                    break;
                }
                result[i][j] = (int)sum;
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[10][10], int result[10][10]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main() {
    enum Operation op = get_operation();
    if (op == OP_INVALID) {
        printf("Invalid operation selected.\n");
        return 1;
    }
    
    int rows1, cols1, rows2, cols2;
    int matrix1[10][10] = {0};
    int matrix2[10][10] = {0};
    int result[10][10] = {0};
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            rows1 = read_matrix_size("Enter rows for matrices: ");
            cols1 = read_matrix_size("Enter columns for matrices: ");
            if (rows1 == -1 || cols1 == -1) {
                printf("Invalid matrix dimensions.\n");
                return 1;
            }
            printf