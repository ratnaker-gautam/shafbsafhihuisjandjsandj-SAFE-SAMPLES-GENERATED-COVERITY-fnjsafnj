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
    int value;
    char buffer[32];
    
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
    mat->rows = read_int("Rows (1-8): ", 1, MAX_SIZE);
    mat->cols = read_int("Columns (1-8): ", 1, MAX_SIZE);
    
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
    
    int i = 0;
    while (i < a->rows) {
        int j = 0;
        do {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        return -1;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MAX / b->data[k][j]) {
                        return -1;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
            j++;
        } while (j < b->cols);
        i++;
    }
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        int j = 0;
        while (j < src->cols) {
            dest->data[j][i] = src->data[i][j];
            j++;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    read_matrix(&mat1, "A");
    print_matrix(&mat1, "A");
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Multiply with another matrix\n");
        printf("2. Transpose matrix\n");
        printf("3. Exit\n");
        
        choice = read_int("Choose operation (1-3): ", 1, 3);
        
        if (choice == 1) {
            read_matrix(&mat2, "B");
            print_matrix(&mat2, "B");
            
            int mult_result = multiply_matrices(&mat1, &mat2, &result);
            if (mult_result == 1) {
                printf("Matrix multiplication result:\n");
                print_matrix(&result, "A x B");
            } else if (mult_result == -1) {
                printf("Error: Integer overflow detected during multiplication.\n");
            } else {
                printf("Error: Cannot multiply - dimension mismatch.\n");
            }
        } else if (choice == 2) {
            transpose_matrix(&mat1, &result);
            printf("Transpose of matrix A:\n");
            print_matrix(&result, "A^T");
        } else {
            break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}