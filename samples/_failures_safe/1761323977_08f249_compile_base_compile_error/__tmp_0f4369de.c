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

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[256];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) return min - 1;
        if (sscanf(buffer, "%d", &value) != 1) continue;
        if (value >= min && value <= max) return value;
    }
}

int **read_matrix(int rows, int cols) {
    int **matrix = allocate_matrix(rows, cols);
    if (!matrix) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter element: ", INT_MIN, INT_MAX);
        }
    }
    return matrix;
}

int main() {
    enum MatrixOp op;
    int rows1, cols1, rows2, cols2;
    int **mat1 = NULL, **mat2 = NULL, **result = NULL;

    while (1) {
        printf("Matrix Operations:\n0: Add\n1: Subtract\n2: Multiply\n3: Transpose\n4: Exit\n");
        int choice = read_int("Select operation: ", 0, 4);
        op = (enum MatrixOp)choice;
        if (op == OP_EXIT) break;

        switch (op) {
            case OP_ADD:
            case OP_SUBTRACT:
                rows1 = read_int("Enter rows for matrix 1: ", 1, 10);
                cols1 = read_int("Enter cols for matrix 1: ", 1, 10);
                rows2 = read_int("Enter rows for matrix 2: ", 1, 10);
                cols2 = read_int("Enter cols for matrix 2: ", 1, 10);
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Matrix dimensions must match for addition/subtraction.\n");
                    continue;
                }
                printf("Enter matrix 1:\n");
                mat1 = read_matrix(rows1, cols1);
                if (!mat1) continue;
                printf("Enter matrix 2:\n");
                mat2 = read_matrix(rows2, cols2);
                if (!mat2) {
                    free_matrix(mat1, rows1);
                    continue;
                }
                result = allocate_matrix(rows1, cols1);
                if (!result) {
                    free_matrix(mat1, rows1);
                    free_matrix(mat2, rows2);
                    continue;
                }
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                        if (op == OP_ADD) {
                            if ((mat2[i][j] > 0 && mat1[i][j] > INT_MAX - mat2[i][j]) ||
                                (mat2[i][j] < 0 && mat1[i][j] < INT_MIN - mat2[i][j])) {
                                printf("Integer overflow detected.\n");
                                free_matrix(mat1, rows1);
                                free_matrix(mat2, rows2);
                                free_matrix(result, rows1);
                                result = NULL;
                                break;
                            }
                            result[i][j] = mat1[i][j] + mat2[i][j];
                        } else {
                            if ((mat2[i][j] < 0 && mat1[i][j] > INT_MAX + mat2[i][j]) ||
                                (mat2[i][j] > 0 && mat1[i][j] < INT_MIN + mat2[i][j])) {
                                printf("Integer overflow detected.\n");
                                free_matrix(mat1, rows1);
                                free_matrix(mat2, rows2);
                                free_matrix(result, rows1);
                                result = NULL;
                                break;
                            }
                            result[i][j] = mat1[i][j] - mat2[i][j];
                        }
                    }
                    if (!result) break;
                }
                if (result) {
                    printf("Result:\n");
                    print_matrix(result, rows1, cols1);
                    free_matrix(result, rows1);
                }
                free_matrix(mat1, rows1);
                free_matrix(mat2, rows2