//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

Matrix multiply_recursive(const Matrix* a, const Matrix* b, int row, int col, int k) {
    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;
    
    if (k < 0) {
        result.data[row][col] = 0;
        return result;
    }
    
    Matrix partial = multiply_recursive(a, b, row, col, k - 1);
    result.data[row][col] = partial.data[row][col] + a->data[row][k] * b->data[k][col];
    
    return result;
}

Matrix matrix_multiply(const Matrix* a, const Matrix* b) {
    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;
    
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            Matrix cell_result = multiply_recursive(a, b, i, j, a->cols - 1);
            result.data[i][j] = cell_result.data[i][j];
        }
    }
    
    return result;
}

int determinant_recursive(const Matrix* mat, int size) {
    if (size == 1) {
        return mat->data[0][0];
    }
    
    if (size == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        Matrix submat;
        submat.rows = size - 1;
        submat.cols = size - 1;
        
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j != col) {
                    submat.data[sub_i][sub_j] = mat->data[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        det += sign * mat->data[0][col] * determinant_recursive(&submat, size - 1);
        sign = -sign;
    }
    
    return det;
}

int matrix_determinant(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    return determinant_recursive(mat, mat->rows);
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int choice;
    do {
        printf("\nAvailable operations:\n");
        printf("1. Matrix Multiplication\n");
        printf("2. Matrix Determinant\n");
        printf("3. Exit\n");
        
        choice = read_int("Select operation (1-3): ", 1, 3);
        
        if (choice == 1) {
            printf("\nMatrix A:\n");
            int rows_a = read_int("Enter rows for matrix A (1-8): ", 1, MAX_SIZE);
            int cols_a = read_int("Enter columns for matrix A (1-8): ", 1, MAX_SIZE);
            
            Matrix a;
            a.rows = rows_a;
            a.cols = cols_a;
            read_matrix(&a);
            
            printf("\nMatrix B:\n");
            int rows_b = read_int("Enter rows for matrix B (1-8): ", 1, MAX_SIZE);
            int cols_b = read_int("Enter columns for matrix B (1-8): ", 1, MAX_SIZE);
            
            if (cols_a != rows_b) {
                printf("Error: Columns of A must equal rows of B for multiplication.\n");
                continue;
            }
            
            Matrix b;