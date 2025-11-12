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

void init_matrix(Matrix* mat) {
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
        do {
            mat->data[i][j] = 0;
            j++;
        } while (j < mat->cols);
        i++;
    }
}

void input_matrix(Matrix* mat) {
    printf("Enter matrix elements (%dx%d):\n", mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
        do {
            printf("%6d", mat->data[i][j]);
            j++;
        } while (j < mat->cols);
        printf("\n");
        i++;
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
        int j = 0;
        while (j < b->cols) {
            int sum = 0;
            int k = 0;
            do {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        return 0;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        return 0;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
                k++;
            } while (k < a->cols);
            result->data[i][j] = sum;
            j++;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    int i = 0;
    while (i < src->rows) {
        int j = 0;
        do {
            dest->data[j][i] = src->data[i][j];
            j++;
        } while (j < src->cols);
        i++;
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    a.rows = read_int("Enter rows for matrix A (1-8): ", 1, MAX_SIZE);
    a.cols = read_int("Enter columns for matrix A (1-8): ", 1, MAX_SIZE);
    input_matrix(&a);
    
    b.rows = read_int("Enter rows for matrix B (1-8): ", 1, MAX_SIZE);
    b.cols = read_int("Enter columns for matrix B (1-8): ", 1, MAX_SIZE);
    input_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Multiplication failed: dimension mismatch or integer overflow.\n");
    }
    
    printf("\nTranspose of Matrix A:\n");
    transpose_matrix(&a, &result);
    print_matrix(&result);
    
    printf("\nTranspose of Matrix B:\n");
    transpose_matrix(&b, &result);
    print_matrix(&result);
    
    return 0;
}