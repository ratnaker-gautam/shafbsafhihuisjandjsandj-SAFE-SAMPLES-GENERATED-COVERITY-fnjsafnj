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
            char prompt[64];
            snprintf(prompt, sizeof(prompt), 
                    "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s:\n", name);
    
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
    printf("\n");
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    init_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int k = 0; k < a->cols; k++) {
            int temp = a->data[i][k];
            int j = 0;
            while (j < b->cols) {
                if (temp > 0 && result->data[i][j] > INT_MAX - temp * b->data[k][j]) {
                    return 0;
                }
                if (temp < 0 && result->data[i][j] < INT_MIN - temp * b->data[k][j]) {
                    return 0;
                }
                result->data[i][j] += temp * b->data[k][j];
                j++;
            }
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
    
    printf("Matrix Operations:\n");
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
            print_matrix(&result, "A^T");
            break;
        case 3:
            transpose_matrix(&b, &result);
            print_matrix(&result, "B^T");
            break;
    }
    
    return 0;
}