//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            continue;
        }
        
        if (temp < min || temp > max) {
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
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
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    return 0;
                }
                sum += product;
            }
            result->data[i][j] = (int)sum;
        }
    }
    
    return 1;
}

int transpose_matrix(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
    
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    printf("Enter dimensions for first matrix:\n");
    mat1.rows = read_int("Rows (1-10): ", 1, MAX_SIZE);
    mat1.cols = read_int("Columns (1-10): ", 1, MAX_SIZE);
    read_matrix(&mat1);
    
    printf("Enter dimensions for second matrix:\n");
    mat2.rows = read_int("Rows (1-10): ", 1, MAX_SIZE);
    mat2.cols = read_int("Columns (1-10): ", 1, MAX_SIZE);
    read_matrix(&mat2);
    
    while (1) {
        printf("\nChoose operation:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose first matrix\n");
        printf("4. Transpose second matrix\n");
        printf("5. Exit\n");
        
        choice = read_int("Enter choice (1-5): ", 1, 5);
        
        if (choice == 5) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (add_matrices(&mat1, &mat2, &result)) {
                    print_matrix(&result);
                } else {
                    printf("Error: Cannot add matrices - dimension mismatch or overflow\n");
                }
                break;
            case 2:
                if (