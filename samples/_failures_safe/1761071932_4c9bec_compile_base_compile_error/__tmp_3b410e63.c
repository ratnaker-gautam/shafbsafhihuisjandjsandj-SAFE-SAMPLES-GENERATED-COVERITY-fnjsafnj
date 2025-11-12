//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: matrix_ops
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

int read_int(const char* prompt, int min, int max) {
    char buffer[256];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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
    mat->rows = read_int("Rows (1-100): ", 1, MAX_DIM);
    mat->cols = read_int("Columns (1-100): ", 1, MAX_DIM);
    
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
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
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > 0 && sum > LLONG_MAX - product) {
                    return -1;
                }
                if (product < 0 && sum < LLONG_MIN - product) {
                    return -1;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                return -1;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < dest->rows; i++) {
        for (int j = 0; j < dest->cols; j++) {
            dest->data[i][j] = src->data[j][i];
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    read_matrix(&a, "A");
    read_matrix(&b, "B");
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Print matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose matrix A\n");
        printf("4. Transpose matrix B\n");
        printf("5. Exit\n");
        
        choice = read_int("Select operation (1-5): ", 1, 5);
        
        switch (choice) {
            case 1:
                print_matrix(&a, "A");
                print_matrix(&b, "B");
                break;
                
            case 2:
                {
                    int status = multiply_matrices(&a, &b, &result);
                    if (status == 1) {
                        printf("Matrix multiplication successful:\n");
                        print_matrix(&result, "A x B");
                    } else if (status == -1) {
                        printf("Error: Integer overflow during multiplication.\n");
                    } else {
                        printf("Error: Cannot multiply - dimension mismatch.\n");
                        printf("Matrix A columns (%d) must equal Matrix B rows (%d).\n", a.cols, b.rows);
                    }
                }
                break;
                
            case 3:
                transpose_matrix(&a, &result);
                printf("Transpose of matrix A:\n");
                print_matrix(&result, "A^T");
                break;
                
            case 4:
                transpose_matrix(&b, &result);
                printf("Transpose of matrix B:\n");
                print_matrix(&result, "B^T");
                break;