//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

void init_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void random_fill_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = rand() % 100;
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

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    init_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += (long long)a->data[i][k] * (long long)b->data[k][j];
                if (sum > INT_MAX || sum < INT_MIN) {
                    return 0;
                }
            }
            result->data[i][j] = (int)sum;
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
    Matrix a, b, result;
    int choice;
    
    srand(42);
    
    printf("Matrix Operations Program\n");
    
    a.rows = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
    a.cols = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
    b.rows = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
    b.cols = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
    
    if (a.rows == -1 || a.cols == -1 || b.rows == -1 || b.cols == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    random_fill_matrix(&a);
    random_fill_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Matrix Multiplication\n");
        printf("2. Transpose Matrix A\n");
        printf("3. Transpose Matrix B\n");
        printf("4. Exit\n");
        
        choice = read_int("Select operation: ", 1, 4);
        if (choice == -1) {
            printf("Input error.\n");
            return 1;
        }
        
        switch (choice) {
            case 1:
                if (multiply_matrices(&a, &b, &result)) {
                    printf("\nMatrix A * B:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrix multiplication failed: dimension mismatch or overflow.\n");
                }
                break;
                
            case 2:
                transpose_matrix(&a, &result);
                printf("\nTranspose of A:\n");
                print_matrix(&result);
                break;
                
            case 3:
                transpose_matrix(&b, &result);
                printf("\nTranspose of B:\n");
                print_matrix(&result);
                break;
                
            case 4:
                return 0;
        }
    }
    
    return 0;
}