//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    printf("Enter rows and columns: ");
    if (scanf("%d %d", rows, cols) != 2 || *rows <= 0 || *cols <= 0) return NULL;
    int **matrix = allocate_matrix(*rows, *cols);
    if (matrix == NULL) return NULL;
    printf("Enter %d elements:\n", (*rows) * (*cols));
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
    enum MatrixOp op;
    int rows1, cols1, rows2, cols2;
    int **mat1 = NULL, **mat2 = NULL, **result = NULL;

    while (1) {
        printf("Choose operation (0=Add,1=Subtract,2=Multiply,3=Transpose,4=Exit): ");
        if (scanf("%d", (int*)&op) != 1 || op < OP_ADD || op > OP_EXIT) {
            printf("Invalid operation\n");
            while (getchar() != '\n');
            continue;
        }

        if (op == OP_EXIT) break;

        switch (op) {
            case OP_ADD:
            case OP_SUBTRACT:
                printf("Matrix 1:\n");
                mat1 = read_matrix(&rows1, &cols1);
                if (mat1 == NULL) {
                    printf("Invalid matrix 1\n");
                    break;
                }
                printf("Matrix 2:\n");
                mat2 = read_matrix(&rows2, &cols2);
                if (mat2 == NULL) {
                    printf("Invalid matrix 2\n");
                    free_matrix(mat1, rows1);
                    break;
                }
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Dimension mismatch\n");
                    free_matrix(mat1, rows1);
                    free_matrix(mat2, rows2);
                    break;
                }
                result = allocate_matrix(rows1, cols1);
                if (result == NULL) {
                    printf("Memory error\n");
                    free_matrix(mat1, rows1);
                    free_matrix(mat2, rows2);
                    break;
                }
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols1; j++) {
                        if (op == OP_ADD) {
                            result[i][j] = mat1[i][j] + mat2[i][j];
                        } else {
                            result[i][j] = mat1[i][j] - mat2[i][j];
                        }
                    }
                }
                printf("Result:\n");
                print_matrix(result, rows1, cols1);
                free_matrix(mat1, rows1);
                free_matrix(mat2, rows2);
                free_matrix(result, rows1);
                break;

            case OP_MULTIPLY:
                printf("Matrix 1:\n");
                mat1 = read_matrix(&rows1, &cols1);
                if (mat1 == NULL) {
                    printf("Invalid matrix 1\n");
                    break;
                }
                printf("Matrix 2:\n");
                mat2 = read_matrix(&rows2, &cols2);
                if (mat2 == NULL) {
                    printf("Invalid matrix 2\n");
                    free_matrix(mat1, rows1);
                    break;
                }
                if (cols1 != rows2) {
                    printf("Dimension mismatch\n");
                    free_matrix(mat1, rows1);
                    free_matrix(mat2, rows2);
                    break;
                }
                result = allocate_matrix(rows1, cols2);
                if (result == NULL) {
                    printf("Memory error\n");
                    free_matrix(mat1, rows1);
                    free_matrix(mat2, rows2);
                    break;
                }
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols2; j++) {
                        result[i][j] = 0