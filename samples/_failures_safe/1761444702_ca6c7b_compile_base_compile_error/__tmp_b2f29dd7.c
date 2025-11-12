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
    OP_EXIT
};

void print_matrix(int **matrix, int rows, int cols) {
    if (matrix == NULL) return;
    for (int i = 0; i < rows; i++) {
        if (matrix[i] == NULL) return;
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int **allocate_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > 100 || cols > 100) {
        return NULL;
    }
    int **matrix = malloc(rows * sizeof(int *));
    if (matrix == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    if (matrix == NULL) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[256];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
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

void read_matrix_values(int **matrix, int rows, int cols) {
    if (matrix == NULL) return;
    for (int i = 0; i < rows; i++) {
        if (matrix[i] == NULL) return;
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter value for [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int **matrix_add(int **a, int **b, int rows, int cols) {
    if (a == NULL || b == NULL) return NULL;
    int **result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        if (a[i] == NULL || b[i] == NULL || result[i] == NULL) {
            free_matrix(result, rows);
            return NULL;
        }
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
    if (a == NULL || b == NULL) return NULL;
    int **result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        if (a[i] == NULL || b[i] == NULL || result[i] == NULL) {
            free_matrix(result, rows);
            return NULL;
        }
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
    if (a == NULL || b == NULL) return NULL;
    int **result = allocate_matrix(rows_a, cols_b);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows_a; i++) {
        if (a[i] == NULL || result[i] == NULL) {
            free_matrix(result, rows_a);
            return NULL;
        }
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_a; k++) {
                if (b[k] == NULL) {
                    free_matrix(result, rows_a);
                    return NULL;
                }
                long long