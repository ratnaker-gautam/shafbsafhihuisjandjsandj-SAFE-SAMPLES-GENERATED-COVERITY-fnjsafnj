//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
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

void read_matrix(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    mat->cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = a->cols;
    
    int i = 0;
    while (i < a->rows) {
        int j = 0;
        do {
            if (j < a->cols) {
                result->data[i][j] = a->data[i][j] + b->data[i][j];
            }
            j++;
        } while (j < a->cols);
        i++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            int k = 0;
            while (k < a->cols) {
                sum += a->data[i][k] * b->data[k][j];
                k++;
            }
            result->data[i][j] = sum;
        }
    }
}

void transpose_matrix(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    int i = 0;
    do {
        int j = 0;
        while (j < mat->rows) {
            result->data[i][j] = mat->data[j][i];
            j++;
        }
        i++;
    } while (i < mat->cols);
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    read_matrix(&mat1, "A");
    read_matrix(&mat2, "B");
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Add matrices (A + B)\n");
        printf("2. Multiply matrices (A * B)\n");
        printf("3. Transpose matrix A\n");
        printf("4. Transpose matrix B\n");
        printf("5. Exit\n");
        
        choice = read_int("Select operation (1-5): ", 1, 5);
        
        if (choice == 5) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                    break;
                }
                add_matrices(&mat1, &mat2, &result);
                print_matrix(&result, "A + B");
                break;
                
            case 2:
                if (mat1.cols != mat2.rows) {
                    printf("Error: Columns of A must equal rows of B for multiplication.\n");
                    break;
                }
                multiply_matrices(&mat1, &mat2, &result);
                print_matrix(&result, "A * B");
                break;
                
            case 3:
                transpose_matrix(&mat1, &result);
                print_matrix(&result, "A^T");
                break;
                
            case 4:
                transpose_matrix(&