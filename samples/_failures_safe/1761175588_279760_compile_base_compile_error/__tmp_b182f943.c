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

int get_int_input(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

void matrix_add(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
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

void matrix_subtract(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
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

void matrix_multiply(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    result[i][j] = 0;
                    break;
                }
                if (result[i][j] > INT_MAX - (int)product || result[i][j] < INT_MIN - (int)product) {
                    result[i][j] = 0;
                    break;
                }
                result[i][j] += (int)product;
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int matrix_determinant_2x2(int matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

int main() {
    int rows, cols;
    
    rows = get_int_input("Enter number of rows (1-5): ", 1, 5);
    if (rows == -1) return 1;
    
    cols = get_int_input("Enter number of columns (1-5): ", 1, 5);
    if (cols == -1) return 1;
    
    int matrix1[5][5] = {0};
    int matrix2[5][5] = {0};
    
    printf("Enter values for first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix1[i][j] = get_int_input("", INT_MIN, INT_MAX);
            if (matrix1[i][j] == -1) return 1;
        }
    }
    
    printf("First matrix:\n");
    print_matrix(rows, cols, matrix1);
    
    int op_choice = get_int_input("Select operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose, 4=Determinant): ", 0,