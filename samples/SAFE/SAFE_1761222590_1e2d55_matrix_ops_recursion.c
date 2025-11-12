//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m.data[i][j] = 0;
        }
    }
    return m;
}

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix add_matrices_recursive(Matrix a, Matrix b, int row, int col) {
    if (row >= a.rows) {
        return a;
    }
    
    Matrix result = a;
    result.data[row][col] = a.data[row][col] + b.data[row][col];
    
    if (col + 1 < a.cols) {
        return add_matrices_recursive(result, b, row, col + 1);
    } else {
        return add_matrices_recursive(result, b, row + 1, 0);
    }
}

Matrix add_matrices(Matrix a, Matrix b) {
    if (a.rows != b.rows || a.cols != b.cols) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    return add_matrices_recursive(a, b, 0, 0);
}

int read_int_safe() {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value < 0 || value > MAX_SIZE) {
        return -1;
    }
    return (int)value;
}

void read_matrix_elements(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            int val = read_int_safe();
            if (val == -1) {
                printf("Invalid input. Using 0.\n");
                m->data[i][j] = 0;
            } else {
                m->data[i][j] = val;
            }
        }
    }
}

int main() {
    printf("Matrix Addition using Recursion\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    printf("Enter rows for matrix A: ");
    int rows_a = read_int_safe();
    if (rows_a <= 0 || rows_a > MAX_SIZE) {
        printf("Invalid rows. Using 2.\n");
        rows_a = 2;
    }
    
    printf("Enter columns for matrix A: ");
    int cols_a = read_int_safe();
    if (cols_a <= 0 || cols_a > MAX_SIZE) {
        printf("Invalid columns. Using 2.\n");
        cols_a = 2;
    }
    
    Matrix a = create_matrix(rows_a, cols_a);
    printf("Enter elements for matrix A:\n");
    read_matrix_elements(&a);
    
    printf("Enter rows for matrix B: ");
    int rows_b = read_int_safe();
    if (rows_b <= 0 || rows_b > MAX_SIZE) {
        printf("Invalid rows. Using %d.\n", rows_a);
        rows_b = rows_a;
    }
    
    printf("Enter columns for matrix B: ");
    int cols_b = read_int_safe();
    if (cols_b <= 0 || cols_b > MAX_SIZE) {
        printf("Invalid columns. Using %d.\n", cols_a);
        cols_b = cols_a;
    }
    
    Matrix b = create_matrix(rows_b, cols_b);
    printf("Enter elements for matrix B:\n");
    read_matrix_elements(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(a);
    
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    Matrix result = add_matrices(a, b);
    if (result.rows == 0) {
        printf("\nMatrices cannot be added - dimension mismatch.\n");
    } else {
        printf("\nResult (A + B):\n");
        print_matrix(result);
    }
    
    return 0;
}