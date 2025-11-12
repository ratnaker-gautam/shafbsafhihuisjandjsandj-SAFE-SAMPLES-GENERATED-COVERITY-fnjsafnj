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

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter value between %d and %d.\n", min_val, max_val);
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
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -100, 100);
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
}

int add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
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
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += (int)product;
                k++;
            } while (k < a->cols);
            result->data[i][j] = sum;
            j++;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
        do {
            result->data[j][i] = mat->data[i][j];
            j++;
        } while (j < mat->cols);
        i++;
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    a.rows = read_int("Enter rows for matrix A (1-8): ", 1, MAX_SIZE);
    a.cols = read_int("Enter columns for matrix A (1-8): ", 1, MAX_SIZE);
    input_matrix(&a, "A");
    
    b.rows = read_int("Enter rows for matrix B (1-8): ", 1, MAX_SIZE);
    b.cols = read_int("Enter columns for matrix B (1-8): ", 1, MAX_SIZE);
    input_matrix(&b, "B");
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n