//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
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
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    return 0;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix* a, Matrix* result) {
    result->rows = a->cols;
    result->cols = a->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = a->data[j][i];
        }
    }
    return 1;
}

int main(void) {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix 1 dimensions:\n");
    mat1.rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    read_matrix(&mat1);
    
    printf("Matrix 2 dimensions:\n");
    mat2.rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    mat2.cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    read_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Matrix Transpose (Matrix 1)\n");
        printf("4. Exit\n");
        
        choice = read_int("Select operation (1-4): ", 1, 4);
        
        if (choice == 4) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (matrix_add(&mat1, &mat2, &result)) {
                    printf("\nMatrix Addition Result:\n");
                    print_matrix(&result);
                }