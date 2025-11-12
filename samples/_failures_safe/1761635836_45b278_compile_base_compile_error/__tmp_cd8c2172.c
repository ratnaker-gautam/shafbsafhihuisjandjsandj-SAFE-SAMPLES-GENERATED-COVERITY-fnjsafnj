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
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        return 0;
    }
    return 1;
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
    Matrix result = create_matrix(a.rows, a.cols);
    
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
        
        break;
    }
    
    return value;
}

void fill_matrix_interactive(Matrix* m) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = get_valid_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    int choice;
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    choice = get_valid_int("Choose operation (1-2): ", 1, 2);
    
    printf("\nMatrix A:\n");
    int rows_a = get_valid_int("Enter rows (1-10): ", 1, MAX_SIZE);
    int cols_a = get_valid_int("Enter columns (1-10): ", 1, MAX_SIZE);
    a = create_matrix(rows_a, cols_a);
    fill_matrix_interactive(&a);
    
    printf("\nMatrix B:\n");
    int rows_b, cols_b;
    
    if (choice == 1) {
        rows_b = rows_a;
        cols_b = cols_a;
        printf("For addition, matrix B must be %dx%d\n", rows_a, cols_a);
    } else {
        rows_b = get_valid_int("Enter rows (1-10): ", 1, MAX_SIZE);
        cols_b = get_valid_int("Enter columns (1-10): ", 1, MAX_SIZE);
        
        if (cols_a != rows_b) {
            printf("Error: For multiplication, columns of A must equal rows of B.\n");
            return 1;
        }
    }
    
    b = create_matrix(rows_b, cols_b);
    fill_matrix_interactive(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(a);
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    if (choice == 1) {
        result = add_matrices_recursive(a, b, 0, 0);
        printf("\nResult of A + B:\n");
    } else {
        result = multiply_matrices_recursive(a, b, 0