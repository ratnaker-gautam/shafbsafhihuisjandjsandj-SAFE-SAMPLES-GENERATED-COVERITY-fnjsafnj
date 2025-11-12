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
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        m.rows = 0;
        m.cols = 0;
        return m;
    }
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
    if (m.rows <= 0 || m.cols <= 0) {
        printf("Invalid matrix dimensions\n");
        return;
    }
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix add_matrices_recursive(Matrix a, Matrix b, int row, int col) {
    Matrix result = create_matrix(a.rows, a.cols);
    
    if (a.rows != b.rows || a.cols != b.cols) {
        return create_matrix(0, 0);
    }
    
    if (row >= a.rows) {
        return result;
    }
    
    if (col >= a.cols) {
        return add_matrices_recursive(a, b, row + 1, 0);
    }
    
    result = add_matrices_recursive(a, b, row, col + 1);
    result.data[row][col] = a.data[row][col] + b.data[row][col];
    return result;
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, int i, int j, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    
    if (a.cols != b.rows) {
        return create_matrix(0, 0);
    }
    
    if (i >= a.rows) {
        return result;
    }
    
    if (j >= b.cols) {
        return multiply_matrices_recursive(a, b, i + 1, 0, 0);
    }
    
    if (k >= a.cols) {
        return multiply_matrices_recursive(a, b, i, j + 1, 0);
    }
    
    result = multiply_matrices_recursive(a, b, i, j, k + 1);
    result.data[i][j] += a.data[i][k] * b.data[k][j];
    return result;
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

void input_matrix(Matrix* m) {
    if (m == NULL || m->rows <= 0 || m->cols <= 0) {
        return;
    }
    printf("Enter matrix elements:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = get_valid_int(prompt, -100, 100);
        }
    }
}

int main() {
    int choice;
    Matrix a, b, result;
    
    printf("Matrix Operations (Recursive Implementation)\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    choice = get_valid_int("Choose operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        printf("\nMatrix Addition\n");
        int rows = get_valid_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
        int cols = get_valid_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
        
        a = create_matrix(rows, cols);
        b = create_matrix(rows, cols);
        
        printf("\nMatrix A:\n");
        input_matrix(&a);
        printf("\nMatrix B:\n");
        input_matrix(&b);
        
        result = add_matrices_recursive(a, b, 0, 0);
        
        printf("\nResult:\n");
        print_matrix(result);
    } else {
        printf("\nMatrix Multiplication\n");
        int rows1 = get_valid_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
        int cols1 = get_valid_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
        int rows2 = get_valid_int("Enter rows