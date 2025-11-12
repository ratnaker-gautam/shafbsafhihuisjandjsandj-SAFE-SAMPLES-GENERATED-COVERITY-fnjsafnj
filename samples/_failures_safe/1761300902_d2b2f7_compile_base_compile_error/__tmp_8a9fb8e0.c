//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

void input_matrix(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows (1-8): ", 1, MAX_SIZE);
    mat->cols = read_int("Columns (1-8): ", 1, MAX_SIZE);
    
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("%s (%d x %d):\n", name, mat->rows, mat->cols);
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
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            int k = 0;
            while (k < a->cols) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (sum > INT_MAX - a->data[i][k] * b->data[k][j]) {
                        return -1;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (sum < INT_MIN - a->data[i][k] * b->data[k][j]) {
                        return -1;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
                k++;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    int i = 0;
    do {
        int j = 0;
        while (j < input->rows) {
            result->data[i][j] = input->data[j][i];
            j++;
        }
        i++;
    } while (i < input->cols);
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    input_matrix(&mat1, "A");
    input_matrix(&mat2, "B");
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Multiply matrices (A * B)\n");
        printf("2. Transpose matrix A\n");
        printf("3. Transpose matrix B\n");
        printf("4. Exit\n");
        
        choice = read_int("Select operation (1-4): ", 1, 4);
        
        switch (choice) {
            case 1: {
                int status = multiply_matrices(&mat1, &mat2, &result);
                if (status == 1) {
                    printf("\nMatrix multiplication result:\n");
                    print_matrix(&result, "A * B");
                } else if (status == -1) {
                    printf("Error: Integer overflow during multiplication.\n");
                } else {
                    printf("Error: Incompatible dimensions for multiplication.\n");
                }
                break;
            }
            case 2:
                transpose_matrix(&mat1, &result);
                printf("\nTranspose of matrix A:\n");
                print_matrix(&result, "A^T");
                break;
            case 3:
                transpose_matrix(&mat2, &result);
                printf("\nTranspose of matrix B:\n");
                print_matrix(&result, "B^T");
                break;
            case 4: