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

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix_recursive(Matrix *m, int row, int col, int value) {
    if (row >= m->rows || col >= m->cols) {
        return;
    }
    m->data[row][col] = value;
    if (col + 1 < m->cols) {
        fill_matrix_recursive(m, row, col + 1, value + 1);
    } else if (row + 1 < m->rows) {
        fill_matrix_recursive(m, row + 1, 0, value + 1);
    }
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, Matrix *result, int i, int j, int k, int sum) {
    if (i >= a.rows) {
        return *result;
    }
    if (j >= b.cols) {
        return multiply_matrices_recursive(a, b, result, i + 1, 0, 0, 0);
    }
    if (k >= a.cols) {
        result->data[i][j] = sum;
        return multiply_matrices_recursive(a, b, result, i, j + 1, 0, 0);
    }
    sum += a.data[i][k] * b.data[k][j];
    return multiply_matrices_recursive(a, b, result, i, j, k + 1, sum);
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    Matrix result = create_matrix(a.rows, b.cols);
    if (a.cols != b.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return result;
    }
    return multiply_matrices_recursive(a, b, &result, 0, 0, 0, 0);
}

int get_valid_int(const char *prompt, int min, int max) {
    char buffer[100];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

int main() {
    printf("Matrix Operations with Recursion\n");
    printf("===============================\n");
    
    int rows1 = get_valid_int("Enter rows for first matrix (1-10): ", 1, MAX_SIZE);
    int cols1 = get_valid_int("Enter columns for first matrix (1-10): ", 1, MAX_SIZE);
    int rows2 = get_valid_int("Enter rows for second matrix (1-10): ", 1, MAX_SIZE);
    int cols2 = get_valid_int("Enter columns for second matrix (1-10): ", 1, MAX_SIZE);
    
    Matrix mat1 = create_matrix(rows1, cols1);
    Matrix mat2 = create_matrix(rows2, cols2);
    
    fill_matrix_recursive(&mat1, 0, 0, 1);
    fill_matrix_recursive(&mat2, 0, 0, 1);
    
    printf("\nFirst matrix:\n");
    print_matrix(mat1);
    printf("\nSecond matrix:\n");
    print_matrix(mat2);
    
    if (mat1.cols == mat2.rows) {
        Matrix product = multiply_matrices(mat1, mat2);
        printf("\nMatrix product:\n");
        print_matrix(product);
    } else {
        printf("\nMatrices cannot be multiplied (columns of first must equal rows of second).\n");
    }
    
    return 0;
}