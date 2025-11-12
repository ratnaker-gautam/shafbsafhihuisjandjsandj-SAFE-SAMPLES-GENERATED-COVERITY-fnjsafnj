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

enum Operation get_operation_choice() {
    int choice;
    printf("Select operation:\n");
    printf("0 - Add matrices\n");
    printf("1 - Subtract matrices\n");
    printf("2 - Multiply matrices\n");
    printf("3 - Transpose matrix\n");
    printf("Enter choice (0-3): ");
    
    if (scanf("%d", &choice) != 1) {
        return OP_INVALID;
    }
    
    if (choice < 0 || choice > 3) {
        return OP_INVALID;
    }
    
    return (enum Operation)choice;
}

int read_matrix_size(const char* prompt, int* rows, int* cols) {
    printf("%s", prompt);
    if (scanf("%d %d", rows, cols) != 2) {
        return 0;
    }
    if (*rows <= 0 || *cols <= 0 || *rows > 100 || *cols > 100) {
        return 0;
    }
    return 1;
}

int read_matrix(int rows, int cols, int matrix[100][100]) {
    printf("Enter %d x %d matrix:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(int rows, int cols, int matrix[100][100]) {
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

void matrix_add(int rows, int cols, int a[100][100], int b[100][100], int result[100][100]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = safe_add(a[i][j], b[i][j]);
        }
    }
}

void matrix_subtract(int rows, int cols, int a[100][100], int b[100][100], int result[100][100]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = safe_add(a[i][j], -b[i][j]);
        }
    }
}

void matrix_multiply(int a_rows, int a_cols, int b_rows, int b_cols, 
                    int a[100][100], int b[100][100], int result[100][100]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
                int product = safe_multiply(a[i][k], b[k][j]);
                if (product == 0 && (a[i][k] != 0 && b[k][j] != 0)) {
                    result[i][j] = 0;
                    break;
                }
                result[i][j] = safe_add(result[i][j], product);
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[100][100], int result[100][100]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main() {
    enum Operation op = get_operation_choice();
    if (op == OP_INVALID) {
        printf("Invalid operation choice\n");
        return 1;
    }

    int rows1, cols1, rows2, cols2;
    int matrix1[100][100], matrix2[100][100], result[100][100];

    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            if (!read_matrix_size("Enter rows and columns for first matrix: ", &rows1, &cols1)) {
                printf("Invalid matrix size\n");
                return 1;
            }
            if (!