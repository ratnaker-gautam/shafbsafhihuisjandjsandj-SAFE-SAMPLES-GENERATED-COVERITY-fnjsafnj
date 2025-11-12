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
    
    int next_col = col + 1;
    int next_row = row;
    if (next_col >= a.cols) {
        next_col = 0;
        next_row = row + 1;
    }
    
    return add_matrices_recursive(result, b, next_row, next_col);
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, int i, int j, int k, Matrix result) {
    if (i >= a.rows) {
        return result;
    }
    
    if (j >= b.cols) {
        j = 0;
        i++;
        return multiply_matrices_recursive(a, b, i, j, 0, result);
    }
    
    if (k >= a.cols) {
        j++;
        return multiply_matrices_recursive(a, b, i, j, 0, result);
    }
    
    result.data[i][j] += a.data[i][k] * b.data[k][j];
    return multiply_matrices_recursive(a, b, i, j, k + 1, result);
}

int read_int(const char* prompt, int min, int max) {
    char buffer[100];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
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

void read_matrix_elements(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    printf("Matrix Operations using Recursion\n");
    printf("=================================\n");
    
    int rows1 = read_int("Enter rows for first matrix (1-10): ", 1, MAX_SIZE);
    int cols1 = read_int("Enter columns for first matrix (1-10): ", 1, MAX_SIZE);
    
    Matrix mat1 = create_matrix(rows1, cols1);
    printf("Enter elements for first matrix:\n");
    read_matrix_elements(&mat1);
    
    int rows2 = read_int("Enter rows for second matrix (1-10): ", 1, MAX_SIZE);
    int cols2 = read_int("Enter columns for second matrix (1-10): ", 1, MAX_SIZE);
    
    Matrix mat2 = create_matrix(rows2, cols2);
    printf("Enter elements for second matrix:\n");
    read_matrix_elements(&mat2);
    
    printf("\nFirst matrix:\n");
    print_matrix(mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(mat2);
    
    if (mat1.rows == mat2.rows && mat1.cols == mat2.cols) {
        Matrix sum = add_matrices_recursive(mat1, mat2, 0, 0);
        printf("\nMatrix sum:\n");
        print_matrix(sum);
    } else {
        printf("\nMatrices cannot be added (dimensions don't match).\n");
    }
    
    if (mat1.cols == mat2.rows) {
        Matrix product = create_matrix(mat1.rows, mat2.cols);
        product = multiply_matrices_recursive(mat1, mat2, 0, 0, 0, product);
        printf("\nMatrix product:\n");
        print_matrix(product);
    } else {
        printf("\nMatrices cannot be multiplied (columns of first must equal rows of second).\n");
    }
    
    return 0;
}