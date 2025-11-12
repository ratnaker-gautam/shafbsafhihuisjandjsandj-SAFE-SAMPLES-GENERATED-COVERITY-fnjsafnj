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

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = a->cols;
    
    int i = 0;
    while (i < a->rows) {
        int j = 0;
        do {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
            j++;
        } while (j < a->cols);
        i++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        printf("Multiplication overflow detected.\n");
                        return;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MAX / b->data[k][j]) {
                        printf("Multiplication overflow detected.\n");
                        return;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
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
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    mat1.cols = read_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    read_matrix(&mat1);
    
    mat2.rows = read_int("Enter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    mat2.cols = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    read_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    int choice;
    do {
        printf("\nOperations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose matrix 1\n");
        printf("4. Transpose matrix 2\n");
        printf("5. Exit\n");
        
        choice = read_int("Select operation (1-5): ", 1, 5);
        
        switch (choice) {
            case 1:
                if (mat1.rows == mat2.rows && mat1.cols == mat2.cols) {
                    add_matrices(&mat1, &