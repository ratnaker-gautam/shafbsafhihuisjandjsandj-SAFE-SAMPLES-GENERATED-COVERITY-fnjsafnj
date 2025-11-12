//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

enum matrix_op {
    MATRIX_ADD,
    MATRIX_SUBTRACT,
    MATRIX_MULTIPLY,
    MATRIX_TRANSPOSE
};

void print_matrix(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.2f", matrix[i][j]);
        }
        printf("\n");
    }
}

double **allocate_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;
    double **matrix = malloc(rows * sizeof(double *));
    if (!matrix) return NULL;
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(double));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(double **matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int read_matrix(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%lf", &matrix[i][j]) != 1) return 0;
        }
    }
    return 1;
}

double **matrix_add(double **a, double **b, int rows, int cols) {
    double **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

double **matrix_subtract(double **a, double **b, int rows, int cols) {
    double **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

double **matrix_multiply(double **a, double **b, int rows_a, int cols_a, int cols_b) {
    double **result = allocate_matrix(rows_a, cols_b);
    if (!result) return NULL;
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < cols_a; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

double **matrix_transpose(double **matrix, int rows, int cols) {
    double **result = allocate_matrix(cols, rows);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

int main(void) {
    int op_choice;
    printf("Matrix Operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("Select operation: ");
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (op_choice < 0 || op_choice > 3) {
        printf("Invalid operation\n");
        return 1;
    }

    enum matrix_op operation = (enum matrix_op)op_choice;
    double **matrix_a = NULL;
    double **matrix_b = NULL;
    double **result = NULL;
    int rows_a, cols_a, rows_b, cols_b;

    switch (operation) {
        case MATRIX_ADD:
        case MATRIX_SUBTRACT:
            printf("Enter rows and columns for matrices: ");
            if (scanf("%d %d", &rows_a, &cols_a) != 2 || rows_a <= 0 || cols_a <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            matrix_a = allocate_matrix(rows_a, cols_a);
            matrix_b = allocate_matrix(rows_a, cols_a);
            if (!matrix_a || !matrix_b) {
                printf("Memory allocation failed\n");
                free_matrix(matrix_a, rows_a);
                free_matrix(matrix_b, rows_a);
                return 1;
            }
            printf("Enter first matrix:\n");
            if (!read_matrix(matrix_a, rows_a, cols_a)) {
                printf("Invalid input\n");
                free_matrix(matrix_a, rows_a);
                free_matrix(matrix_b, rows_a);
                return 1;
            }
            printf("