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

void printMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int **allocateMatrix(int rows, int cols) {
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

void freeMatrix(int **matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int **readMatrix(int *rows, int *cols) {
    printf("Enter rows and columns: ");
    if (scanf("%d %d", rows, cols) != 2) return NULL;
    if (*rows <= 0 || *cols <= 0 || *rows > 100 || *cols > 100) return NULL;
    int **matrix = allocateMatrix(*rows, *cols);
    if (!matrix) return NULL;
    printf("Enter matrix elements:\n");
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                freeMatrix(matrix, *rows);
                return NULL;
            }
        }
    }
    return matrix;
}

int **addMatrices(int **a, int **b, int rows, int cols) {
    int **result = allocateMatrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                freeMatrix(result, rows);
                return NULL;
            }
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

int **subtractMatrices(int **a, int **b, int rows, int cols) {
    int **result = allocateMatrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] < INT_MIN + b[i][j]) ||
                (b[i][j] < 0 && a[i][j] > INT_MAX + b[i][j])) {
                freeMatrix(result, rows);
                return NULL;
            }
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

int **multiplyMatrices(int **a, int **b, int rows1, int cols1, int rows2, int cols2) {
    if (cols1 != rows2) return NULL;
    int **result = allocateMatrix(rows1, cols2);
    if (!result) return NULL;
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    freeMatrix(result, rows1);
                    return NULL;
                }
                if (result[i][j] > INT_MAX - product || result[i][j] < INT_MIN - product) {
                    freeMatrix(result, rows1);
                    return NULL;
                }
                result[i][j] += product;
            }
        }
    }
    return result;
}

int **transposeMatrix(int **matrix, int rows, int cols) {
    int **result = allocateMatrix(cols, rows);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

int main() {
    enum MatrixOp op;
    int choice;
    
    while (1) {
        printf("\nMatrix Operations:\n");
        printf("1. Add\n");
        printf("2. Subtract\n");
        printf("3. Multiply\n");
        printf("4. Transpose\n");
        printf("5. Exit\n");
        printf("Select operation: ");
        
        if (scanf("%d", &choice