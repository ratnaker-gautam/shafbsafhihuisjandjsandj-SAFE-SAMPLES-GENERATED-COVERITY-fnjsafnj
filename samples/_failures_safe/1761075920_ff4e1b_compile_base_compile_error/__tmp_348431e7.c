//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE
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

int **read_matrix(int *rows, int *cols) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", rows, cols) != 2 || *rows <= 0 || *cols <= 0) return NULL;
    int **matrix = allocate_matrix(*rows, *cols);
    if (matrix == NULL) return NULL;
    printf("Enter %d elements row by row:\n", *rows * *cols);
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                free_matrix(matrix, *rows);
                return NULL;
            }
        }
    }
    return matrix;
}

int **matrix_add(int **a, int **b, int rows, int cols) {
    int **result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

int **matrix_subtract(int **a, int **b, int rows, int cols) {
    int **result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

int **matrix_multiply(int **a, int **b, int rows_a, int cols_a, int cols_b) {
    int **result = allocate_matrix(rows_a, cols_b);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_a; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

int **matrix_transpose(int **matrix, int rows, int cols) {
    int **result = allocate_matrix(cols, rows);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

int main(void) {
    enum MatrixOp op;
    int choice;
    printf("Matrix Operations:\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Transpose\n");
    printf("Select operation (1-4): ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        printf("Invalid choice\n");
        return 1;
    }
    op = (enum MatrixOp)(choice - 1);

    int rows_a, cols_a, rows_b, cols_b;
    int **matrix_a = NULL;
    int **matrix_b = NULL;
    int **result = NULL;

    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            matrix_a = read_matrix(&rows_a, &cols_a);
            if (matrix_a == NULL) {
                printf("Invalid first matrix\n");
                return 1;
            }
            matrix_b = read_matrix(&rows_b, &cols_b);
            if (matrix_b == NULL) {
                printf("Invalid second matrix\n");
                free_matrix(matrix_a, rows_a);
                return 1;
            }
            if (rows_a != rows_b || cols_a != cols_b) {
                printf("Matrix dimensions must match\n");
                free_matrix(matrix_a, rows_a);
                free_matrix(matrix_b, rows_b);
                return 1;