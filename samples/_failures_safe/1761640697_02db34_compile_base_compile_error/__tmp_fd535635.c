//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int **allocate_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;
    int **matrix = malloc(rows * sizeof(int *));
    if (!matrix) return NULL;
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int read_int_safe(const char *prompt, int min, int max) {
    int value;
    char buffer[256];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) return min;
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void read_matrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int_safe("Enter element: ", INT_MIN, INT_MAX);
        }
    }
}

int **matrix_add(int **a, int **b, int rows, int cols) {
    int **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                free_matrix(result, rows);
                return NULL;
            }
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

int **matrix_subtract(int **a, int **b, int rows, int cols) {
    int **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] < INT_MIN + b[i][j]) ||
                (b[i][j] < 0 && a[i][j] > INT_MAX + b[i][j])) {
                free_matrix(result, rows);
                return NULL;
            }
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

int **matrix_multiply(int **a, int **b, int rows_a, int cols_a, int cols_b) {
    int **result = allocate_matrix(rows_a, cols_b);
    if (!result) return NULL;
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_a; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    free_matrix(result, rows_a);
                    return NULL;
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    free_matrix(result, rows_a);
                    return NULL;
                }
                result[i][j] = (int)sum;
            }
        }
    }
    return result;
}

int **matrix_transpose(int **matrix, int rows, int cols) {
    int **result = allocate_matrix(cols, rows);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

int matrix_determinant(int **matrix, int n) {
    if (n == 1) return matrix[0][0];
    if (n == 2) {
        long long det = (long long)matrix[0][0] * matrix[1][1] - (long