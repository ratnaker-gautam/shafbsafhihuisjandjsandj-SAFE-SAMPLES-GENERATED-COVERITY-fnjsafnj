//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 10

typedef void (*MatrixOperation)(double[MAX_SIZE][MAX_SIZE], double[MAX_SIZE][MAX_SIZE], double[MAX_SIZE][MAX_SIZE], int, int);
typedef double (*ElementOperation)(double, double);

double add_elements(double a, double b) {
    return a + b;
}

double multiply_elements(double a, double b) {
    return a * b;
}

void apply_element_operation(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols, ElementOperation op) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = op(A[i][j], B[i][j]);
        }
    }
}

void matrix_add(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    apply_element_operation(A, B, result, rows, cols, add_elements);
}

void matrix_multiply_elementwise(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    apply_element_operation(A, B, result, rows, cols, multiply_elements);
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
    int operation;

    printf("Enter first matrix dimensions and elements (rows cols elements):\n");
    if (!read_matrix(A, &rowsA, &colsA)) {
        printf("Invalid first matrix input\n");
        return 1;
    }

    printf("Enter second matrix dimensions and elements (rows cols elements):\n");
    if (!read_matrix(B, &rowsB, &colsB)) {
        printf("Invalid second matrix input\n");
        return 1;
    }

    if (rowsA != rowsB || colsA != colsB) {
        printf("Matrix dimensions must match for operations\n");
        return 1;
    }

    printf("Select operation (1=Add, 2=Multiply elementwise): ");
    if (scanf("%d", &operation) != 1) {
        printf("Invalid operation selection\n");
        return 1;
    }

    MatrixOperation operations[] = {matrix_add, matrix_multiply_elementwise};
    const char* operation_names[] = {"Addition", "Element-wise Multiplication"};

    if (operation < 1 || operation > 2) {
        printf("Invalid operation\n");
        return 1;
    }

    operations[operation-1](A, B, result, rowsA, colsA);

    printf("\nMatrix A:\n");
    print_matrix(A, rowsA, colsA);
    printf("\nMatrix B:\n");
    print_matrix(B, rowsA, colsA);
    printf("\nResult (%s):\n", operation_names[operation-1]);
    print_matrix(result, rowsA, colsA);

    return 0;
}