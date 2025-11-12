//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define MAX_SIZE 10

typedef void (*MatrixOperation)(double[MAX_SIZE][MAX_SIZE], double[MAX_SIZE][MAX_SIZE], double[MAX_SIZE][MAX_SIZE], int, int);

void matrix_add(double a[MAX_SIZE][MAX_SIZE], double b[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void matrix_subtract(double a[MAX_SIZE][MAX_SIZE], double b[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void matrix_multiply(double a[MAX_SIZE][MAX_SIZE], double b[MAX_SIZE][MAX_SIZE], double result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < cols; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int read_matrix(double matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", rows) != 1 || *rows <= 0 || *rows > MAX_SIZE) {
        return 0;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", cols) != 1 || *cols <= 0 || *cols > MAX_SIZE) {
        return 0;
    }
    
    printf("Enter matrix elements row by row:\n");
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
    double matrix_a[MAX_SIZE][MAX_SIZE];
    double matrix_b[MAX_SIZE][MAX_SIZE];
    double result[MAX_SIZE][MAX_SIZE];
    int rows_a, cols_a, rows_b, cols_b;
    
    MatrixOperation operations[] = {matrix_add, matrix_subtract, matrix_multiply};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication"};
    int num_operations = 3;
    
    printf("Matrix A:\n");
    if (!read_matrix(matrix_a, &rows_a, &cols_a)) {
        printf("Invalid input for matrix A\n");
        return 1;
    }
    
    printf("Matrix B:\n");
    if (!read_matrix(matrix_b, &rows_b, &cols_b)) {
        printf("Invalid input for matrix B\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_operations) {
        printf("Invalid operation choice\n");
        return 1;
    }
    
    MatrixOperation selected_op = operations[choice - 1];
    
    if (choice == 3) {
        if (cols_a != rows_b) {
            printf("Matrix multiplication requires columns of A (%d) to equal rows of B (%d)\n", cols_a, rows_b);
            return 1;
        }
        memset(result, 0, sizeof(result));
        selected_op(matrix_a, matrix_b, result, rows_a, cols_b);
        printf("\nResult of multiplication:\n");
        print_matrix(result, rows_a, cols_b);
    } else {
        if (rows_a != rows_b || cols_a != cols_b) {
            printf("Matrix dimensions must match for addition/subtraction\n");
            return 1;
        }
        memset(result, 0, sizeof(result));
        selected_op(matrix_a, matrix_b, result, rows_a, cols_a);
        printf("\nResult of %s:\n", operation_names[choice - 1]);
        print_matrix(result, rows_a, cols_a);
    }
    
    return 0;
}