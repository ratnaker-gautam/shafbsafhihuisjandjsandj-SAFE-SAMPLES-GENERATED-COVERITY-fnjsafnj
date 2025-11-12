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
            return -1;
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
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
    mat->rows = read_int("Rows: ", 1, MAX_DIM);
    mat->cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Enter elements for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
        while (j < mat->cols) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
            j++;
        }
        i++;
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
    
    for (int i = 0; i < result->rows; i++) {
        int j = 0;
        while (j < result->cols) {
            result->data[i][j] = 0;
            int k = 0;
            do {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX - result->data[i][j]) {
                        return 0;
                    }
                }
                if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN - result->data[i][j]) {
                        return 0;
                    }
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
                k++;
            } while (k < a->cols);
            j++;
        }
    }
    
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    int i = 0;
    do {
        int j = 0;
        while (j < src->rows) {
            dest->data[i][j] = src->data[j][i];
            j++;
        }
        i++;
    } while (i < src->cols);
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
        printf("1. Multiply matrices (A * B)\n");
        printf("2. Transpose matrix A\n");
        printf("3. Transpose matrix B\n");
        printf("4. Display matrices\n");
        printf("5. Exit\n");
        
        choice = read_int("Choose operation: ", 1, 5);
        
        switch (choice) {
            case 1:
                if (multiply_matrices(&a, &b, &result)) {
                    printf("Matrix multiplication successful:\n");
                    print_matrix(&result, "A * B");
                } else {
                    printf("Matrix multiplication failed: incompatible dimensions or overflow\n");
                }
                break;
                
            case 2:
                transpose_matrix(&a, &result);
                printf("Transpose of matrix A:\n");
                print_matrix(&result, "A^T");
                break;
                
            case 3:
                transpose_matrix(&b, &result);
                printf("Transpose of matrix B:\n");
                print_matrix(&result, "B^T");
                break;
                
            case 4:
                print_matrix(&a, "A");
                print_matrix(&b, "B");
                break;
                
            case 5: