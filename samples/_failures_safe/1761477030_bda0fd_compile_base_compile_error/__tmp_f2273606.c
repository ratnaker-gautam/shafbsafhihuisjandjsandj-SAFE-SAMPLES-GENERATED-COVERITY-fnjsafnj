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

void init_matrix(Matrix* mat) {
    int i, j;
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void input_matrix(Matrix* mat, const char* name) {
    int i, j;
    printf("Enter values for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    
    i = 0;
    while (i < mat->rows) {
        j = 0;
        while (j < mat->cols) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), 
                    "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
            j++;
        }
        i++;
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    int i, j;
    printf("Matrix %s:\n", name);
    
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int i, j, k;
    
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    init_matrix(result);
    
    i = 0;
    do {
        j = 0;
        do {
            k = 0;
            while (k < a->cols) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        return 0;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        return 0;
                    }
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
                k++;
            }
            j++;
        } while (j < result->cols);
        i++;
    } while (i < result->rows);
    
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    int i, j;
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (i = 0; i < src->rows; i++) {
        for (j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    mat1.rows = read_int("Enter rows for matrix A (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix A (1-10): ", 1, MAX_DIM);
    input_matrix(&mat1, "A");
    
    mat2.rows = read_int("Enter rows for matrix B (1-10): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix B (1-10): ", 1, MAX_DIM);
    input_matrix(&mat2, "B");
    
    while (1) {
        printf("\nMatrix Operations:\n");
        printf("1. Multiply matrices\n");
        printf("2. Transpose matrix A\n");
        printf("3. Transpose matrix B\n");
        printf("4. Exit\n");
        
        choice = read_int("Select operation: ", 1, 4);
        
        switch (choice) {
            case 1:
                if (multiply_matrices(&mat1, &mat2, &result)) {
                    print_matrix(&result, "A x B");
                } else {
                    printf("Matrix multiplication failed: dimension mismatch or integer overflow.\n");
                }
                break;
            case 2:
                transpose_matrix(&mat1, &result);
                print_matrix(&result, "A^T");
                break;
            case