//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 10

typedef void (*MatrixOp)(double[MAX_SIZE][MAX_SIZE], double[MAX_SIZE][MAX_SIZE], double[MAX_SIZE][MAX_SIZE], int, int);
typedef double (*ElementOp)(double, double);

double add_elements(double a, double b) {
    return a + b;
}

double multiply_elements(double a, double b) {
    return a * b;
}

void elementwise_operation(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols, ElementOp op) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = op(A[i][j], B[i][j]);
        }
    }
}

void matrix_add(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    elementwise_operation(A, B, result, rows, cols, add_elements);
}

void matrix_multiply_elementwise(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    elementwise_operation(A, B, result, rows, cols, multiply_elements);
}

void matrix_transpose(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            result[i][j] = A[j][i];
        }
    }
}

int read_matrix(double matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    if (scanf("%d %d", rows, cols) != 2) {
        return 0;
    }
    if (*rows <= 0 || *rows > MAX_SIZE || *cols <= 0 || *cols > MAX_SIZE) {
        return 0;
    }
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%lf", &matrix[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(double matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.2f", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    double A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], result[MAX_SIZE][MAX_SIZE];
    int rowsA, colsA, rowsB, colsB;
    int choice;

    printf("Enter dimensions and elements for matrix A (rows cols then elements):\n");
    if (!read_matrix(A, &rowsA, &colsA)) {
        printf("Invalid input for matrix A\n");
        return 1;
    }

    printf("Enter dimensions and elements for matrix B (rows cols then elements):\n");
    if (!read_matrix(B, &rowsB, &colsB)) {
        printf("Invalid input for matrix B\n");
        return 1;
    }

    printf("Select operation:\n");
    printf("1. Add matrices\n");
    printf("2. Multiply element-wise\n");
    printf("3. Transpose matrix A\n");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }

    MatrixOp operations[] = {matrix_add, matrix_multiply_elementwise, matrix_transpose};
    int num_ops = 3;

    if (choice < 1 || choice > num_ops) {
        printf("Invalid choice\n");
        return 1;
    }

    MatrixOp selected_op = operations[choice - 1];
    int result_rows, result_cols;

    switch (choice) {
        case 1:
            if (rowsA != rowsB || colsA != colsB) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            result_rows = rowsA;
            result_cols = colsA;
            break;
        case 2:
            if (rowsA != rowsB || colsA != colsB) {
                printf("Matrix dimensions must match for element-wise multiplication\n");
                return 1;
            }
            result_rows = rowsA;
            result_cols = colsA;
            break;
        case 3:
            result_rows = colsA;
            result_cols = rowsA;
            break;
        default:
            return 1;
    }

    selected_op(A, B, result, rowsA, colsA);
    printf("Result:\n");
    print_matrix(result, result_rows, result_cols);

    return 0;
}