//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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
    int rows_a, cols_a, rows_b, cols_b;
    double **matrix_a = NULL;
    double **matrix_b = NULL;
    double **result = NULL;

    printf("Matrix Operations Program\n");
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("Select operation (0-3): ");
    
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (op_choice < 0 || op_choice > 3) {
        printf("Invalid operation\n");
        return 1;
    }

    switch (op_choice) {
        case MATRIX_ADD:
        case MATRIX_SUBTRACT:
            printf("Enter dimensions for both matrices (rows columns): ");
            if (scanf("%d %d", &rows_a, &cols_a) != 2) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows_a <= 0 || cols_a <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }
            rows_b = rows_a;
            cols_b = cols_a;
            break;
            
        case MATRIX_MULTIPLY:
            printf("Enter dimensions for first matrix (rows columns): ");
            if (scanf("%d %d", &rows_a, &cols_a) != 2) {
                printf("Invalid dimensions\n");
                return 1;
            }
            if (rows_a <= 0 || cols_a <= 0) {
                printf("Invalid dimensions\n");
                return 1;
            }