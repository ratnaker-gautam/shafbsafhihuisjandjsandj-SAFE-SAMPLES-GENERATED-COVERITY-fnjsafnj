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
    
    if (col >= a.cols) {
        return add_matrices_recursive(a, b, row + 1, 0);
    }
    
    a.data[row][col] += b.data[row][col];
    return add_matrices_recursive(a, b, row, col + 1);
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, Matrix result, int i, int j, int k) {
    if (i >= a.rows) {
        return result;
    }
    
    if (j >= b.cols) {
        return multiply_matrices_recursive(a, b, result, i + 1, 0, 0);
    }
    
    if (k >= a.cols) {
        return multiply_matrices_recursive(a, b, result, i, j + 1, 0);
    }
    
    result.data[i][j] += a.data[i][k] * b.data[k][j];
    return multiply_matrices_recursive(a, b, result, i, j, k + 1);
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
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

void fill_matrix_recursive(Matrix* m, int row, int col) {
    if (row >= m->rows) {
        return;
    }
    
    if (col >= m->cols) {
        fill_matrix_recursive(m, row + 1, 0);
        return;
    }
    
    char prompt[50];
    snprintf(prompt, sizeof(prompt), "Enter value for [%d][%d]: ", row, col);
    m->data[row][col] = get_valid_int(prompt, -100, 100);
    fill_matrix_recursive(m, row, col + 1);
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = get_valid_int("Enter rows for first matrix (1-10): ", 1, MAX_SIZE);
    int cols1 = get_valid_int("Enter columns for first matrix (1-10): ", 1, MAX_SIZE);
    
    printf("\nFilling first matrix:\n");
    Matrix mat1 = create_matrix(rows1, cols1);
    fill_matrix_recursive(&mat1, 0, 0);
    
    int rows2 = get_valid_int("Enter rows for second matrix (1-10): ", 1, MAX_SIZE);
    int cols2 = get_valid_int("Enter columns for second matrix (1-10): ", 1, MAX_SIZE);
    
    printf("\nFilling second matrix:\n");
    Matrix mat2 = create_matrix(rows2, cols2);
    fill_matrix_recursive(&mat2, 0, 0);
    
    printf("\nFirst matrix:\n");
    print_matrix(mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(mat2);
    
    if (mat1.rows == mat2.rows && mat1.cols == mat2.cols) {
        Matrix sum = mat1;
        sum = add_matrices_recursive(sum, mat2, 0, 0);
        printf("\nMatrix sum:\n");
        print_matrix(sum);
    } else {
        printf("\nMatrices cannot be added (different dimensions).\n");
    }
    
    if (mat1.cols == mat2.rows) {
        Matrix product = create_matrix(mat1.rows, mat2.cols);
        product = multiply_matrices_recursive(mat1, mat2, product, 0, 0, 0);
        printf("\nMatrix product:\n");
        print_matrix(product);
    } else {
        printf("\nMatrices cannot be multiplied (incompatible dimensions).\n");
    }
    
    return 0;
}