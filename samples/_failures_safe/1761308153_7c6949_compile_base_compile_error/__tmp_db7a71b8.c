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

void input_matrix(Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        int j = 0;
        while (j < mat->cols) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -1000, 1000);
            j++;
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s:\n", name);
    
    int i = 0;
    do {
        printf("  ");
        int j = 0;
        while (j < mat->cols) {
            printf("%6d", mat->data[i][j]);
            j++;
        }
        printf("\n");
        i++;
    } while (i < mat->rows);
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
    printf("=========================\n\n");
    
    a.rows = read_int("Enter rows for matrix A (1-10): ", 1, MAX_DIM);
    a.cols = read_int("Enter columns for matrix A (1-10): ", 1, MAX_DIM);
    input_matrix(&a, "A");
    
    b.rows = read_int("Enter rows for matrix B (1-10): ", 1, MAX_DIM);
    b.cols = read_int("Enter columns for matrix B (1-10): ", 1, MAX_DIM);
    input_matrix(&b, "B");
    
    printf("\n");
    print_matrix(&a, "A");
    print_matrix(&b, "B");
    
    printf("\nMatrix Operations:\n");
    printf("1. Multiply A * B\n");
    printf("2. Transpose A\n");
    printf("3. Transpose B\n");
    
    int choice = read_int("Select operation (1-3): ", 1, 3);
    
    switch (choice) {
        case 1:
            if (multiply_matrices(&a, &b, &result)) {
                print_matrix(&result, "A * B");
            } else {
                printf("Matrix multiplication failed: dimension mismatch or integer overflow.\n");
            }
            break;
        case 2:
            transpose_matrix(&a, &result);
            print_matrix