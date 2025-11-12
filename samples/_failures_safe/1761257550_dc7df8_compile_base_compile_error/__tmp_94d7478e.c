//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
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

void init_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void input_matrix(Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s:\n", name);
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    init_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Enter dimensions for first matrix:\n");
    mat1.rows = read_int("  Rows (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("  Columns (1-10): ", 1, MAX_DIM);
    
    printf("Enter dimensions for second matrix:\n");
    mat2.rows = read_int("  Rows (1-10): ", 1, MAX_DIM);
    mat2.cols = read_int("  Columns (1-10): ", 1, MAX_DIM);
    
    input_matrix(&mat1, "A");
    input_matrix(&mat2, "B");
    
    printf("\n");
    print_matrix(&mat1, "A");
    print_matrix(&mat2, "B");
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Exit\n");
        
        choice = read_int("Select operation (1-3): ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (mat1.rows == mat2.rows && mat1.cols == mat2.cols) {
                    add_matrices(&mat1, &mat2, &result);
                    print_matrix(&result, "A + B");
                } else {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                }
                break;
                
            case 2:
                if (multiply_matrices(&mat1, &mat2, &result)) {
                    print_matrix(&result, "A * B");
                } else {
                    printf("Error: Incompatible dimensions for multiplication.\n");
                    printf("First matrix columns (%d) must equal second matrix rows (%d).\n", 
                           mat1.cols, mat2.rows);
                }
                break;
        }
    }