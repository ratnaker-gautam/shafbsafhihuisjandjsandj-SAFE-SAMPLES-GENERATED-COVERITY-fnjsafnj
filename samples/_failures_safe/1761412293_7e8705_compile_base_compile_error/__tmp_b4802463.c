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

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long result;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            exit(EXIT_FAILURE);
        }
        
        result = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (result < min_val || result > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)result;
    }
}

void input_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = get_valid_int(prompt, -1000, 1000);
        }
    }
}

void matrix_add(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow detected.\n");
                exit(EXIT_FAILURE);
            }
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void matrix_subtract(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] < INT_MIN + b[i][j]) ||
                (b[i][j] < 0 && a[i][j] > INT_MAX + b[i][j])) {
                printf("Integer overflow detected.\n");
                exit(EXIT_FAILURE);
            }
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void matrix_multiply(int a_rows, int a_cols, int b_cols, int a[a_rows][a_cols], int b[a_cols][b_cols], int result[a_rows][b_cols]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected.\n");
                    exit(EXIT_FAILURE);
                }
                if ((result[i][j] > 0 && product > INT_MAX - result[i][j]) ||
                    (result[i][j] < 0 && product < INT_MIN - result[i][j])) {
                    printf("Integer overflow detected.\n");
                    exit(EXIT_FAILURE);
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
    long det = (long)matrix[0][0] * (long)matrix[1][1] - (long)matrix[0][1] * (long)matrix[1][0];
    if (det > INT_MAX || det < INT_MIN) {
        printf("Integer overflow detected.\n");
        exit(EXIT_FAILURE);
    }
    return (int)det;
}

int main(void) {
    enum Operation op;
    int choice;
    
    printf("Matrix Operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("