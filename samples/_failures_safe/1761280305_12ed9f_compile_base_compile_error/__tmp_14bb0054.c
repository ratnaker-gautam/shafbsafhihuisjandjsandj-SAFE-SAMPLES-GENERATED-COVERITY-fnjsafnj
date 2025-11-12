//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
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
    printf("Enter %d elements row by row:\n", (*rows) * (*cols));
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

int main(void) {
    enum MatrixOp operation;
    int choice;
    printf("Matrix Operations:\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Transpose\n");
    printf("Select operation (1-4): ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        printf("Invalid operation choice\n");
        return 1;
    }
    operation = (enum MatrixOp)(choice - 1);

    int rows1, cols1, rows2, cols2;
    int **matrix1 = read_matrix(&rows1, &cols1);
    if (matrix1 == NULL) {
        printf("Failed to read first matrix\n");
        return 1;
    }

    int **matrix2 = NULL;
    int **result = NULL;

    switch (operation) {
        case OP_ADD:
        case OP_SUBTRACT:
            matrix2 = read_matrix(&rows2, &cols2);
            if (matrix2 == NULL) {
                printf("Failed to read second matrix\n");
                free_matrix(matrix1, rows1);
                return 1;
            }
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition/subtraction\n");
                free_matrix(matrix1, rows1);
                free_matrix(matrix2, rows2);
                return 1;
            }
            result = allocate_matrix(rows1, cols1);
            if (result == NULL) {
                printf("Memory allocation failed\n");
                free_matrix(matrix1, rows1);
                free_matrix(matrix2, rows2);
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (operation == OP_ADD) {
                        result[i][j] = matrix1[i][j] + matrix2[i][j];
                    } else {
                        result[i][j] = matrix1[i][j] - matrix2[i][j];
                    }
                }
            }
            printf("Result:\n");
            print_matrix(result, rows1, cols1);
            break;

        case OP_MULTIPLY:
            matrix2 = read_matrix(&rows2, &cols2);
            if (matrix2 == NULL) {
                printf("Failed to read second matrix\n");
                free_matrix(matrix1, rows1);
                return 1;
            }
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                free_matrix(matrix1, rows1);
                free_matrix(matrix2, rows2);
                return 1;
            }
            result = allocate_matrix(rows1, cols2);
            if (result == NULL) {
                printf("Memory allocation failed\n");
                free_matrix(matrix1, rows1);
                free_matrix(matrix2, rows2);
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                }
            }
            printf("Result:\n");
            print_matrix(result, rows1, cols2);
            break;

        case OP_TRANSPOSE:
            result = allocate_matrix