//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
    int sum = a.data[row][col] + b.data[row][col];
    if ((a.data[row][col] > 0 && b.data[row][col] > INT_MAX - a.data[row][col]) ||
        (a.data[row][col] < 0 && b.data[row][col] < INT_MIN - a.data[row][col])) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    result.data[row][col] = sum;
    
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

Matrix multiply_matrices_recursive(Matrix a, Matrix b, Matrix result, int i, int j, int k, int sum) {
    if (i >= a.rows) {
        return result;
    }
    
    if (j >= b.cols) {
        return multiply_matrices_recursive(a, b, result, i + 1, 0, 0, 0);
    }
    
    if (k >= a.cols) {
        result.data[i][j] = sum;
        return multiply_matrices_recursive(a, b, result, i, j + 1, 0, 0);
    }
    
    int product = a.data[i][k] * b.data[k][j];
    if ((product > 0 && sum > INT_MAX - product) || (product < 0 && sum < INT_MIN - product)) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    
    return multiply_matrices_recursive(a, b, result, i, j, k + 1, sum + product);
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    
    Matrix result = create_matrix(a.rows, b.cols);
    return multiply_matrices_recursive(a, b, result, 0, 0, 0, 0);
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

void fill_matrix(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int("", -1000, 1000);
        }
    }
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    int cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    Matrix mat1 = create_matrix(rows1, cols1);
    printf("Fill matrix 1:\n");
    fill_matrix(&mat1);
    
    int rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    int cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    Matrix mat2 = create_matrix(rows2, cols2);
    printf("Fill matrix 2:\n");
    fill_matrix(&