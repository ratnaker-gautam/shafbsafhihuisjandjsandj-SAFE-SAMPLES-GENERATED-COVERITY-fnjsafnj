//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

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
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

void initialize_matrix(Matrix* mat) {
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
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    initialize_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

void transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Enter dimensions for first matrix:\n");
    mat1.rows = read_int("  Rows (1-10): ", 1, MAX_SIZE);
    mat1.cols = read_int("  Columns (1-10): ", 1, MAX_SIZE);
    input_matrix(&mat1, "A");
    
    printf("Enter dimensions for second matrix:\n");
    mat2.rows = read_int("  Rows (1-10): ", 1, MAX_SIZE);
    mat2.cols = read_int("  Columns (1-10): ", 1, MAX_SIZE);
    input_matrix(&mat2, "B");
    
    while (1) {
        printf("\nAvailable Operations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose matrix A\n");
        printf("4. Transpose matrix B\n");
        printf("5. Exit\n");
        
        choice = read_int("Select operation (1-5): ", 1, 5);
        
        if (choice == 5) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (add_matrices(&mat1, &mat2, &result)) {
                    print_matrix(&result, "A + B");
                } else {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                }
                break;
                
            case 2:
                if (multiply_matrices(&mat1