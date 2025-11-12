//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

void matrix_init(Matrix* mat) {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            mat->data[i][j] = 0;
        }
    }
    mat->rows = 0;
    mat->cols = 0;
}

int matrix_input(Matrix* mat) {
    mat->rows = read_int("Enter number of rows (1-10): ", 1, MAX_ROWS);
    if (mat->rows == -1) return 0;
    
    mat->cols = read_int("Enter number of columns (1-10): ", 1, MAX_COLS);
    if (mat->cols == -1) return 0;
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) return 0;
            mat->data[i][j] = val;
        }
    }
    return 1;
}

void matrix_print(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
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
                    printf("Integer overflow detected during multiplication.\n");
                    return 0;
                }
                sum += product;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    matrix_init(&mat1);
    matrix_init(&mat2);
    matrix_init(&result);
    
    printf("Matrix Operations Program\n");
    printf("1. Input matrices\n");
    printf("2. Multiply matrices\n");
    printf("3. Transpose matrix\n");
    printf("4. Exit\n");
    
    while (1) {
        choice = read_int("Select operation: ", 1, 4);
        if (choice == -1) break;
        
        switch (choice) {
            case 1:
                printf("Matrix 1:\n");
                if (!matrix_input(&mat1)) return 1;
                printf("Matrix 2:\n");
                if (!matrix_input(&mat2)) return 1;
                break;
                
            case 2:
                if (mat1.rows == 0 || mat2.rows == 0) {
                    printf("Please input matrices first.\n");
                    break;
                }
                if (matrix_multiply(&mat1, &mat2, &result)) {
                    printf("Result of multiplication:\n");
                    matrix_print(&result);
                }
                break;
                
            case 3:
                if (mat1.rows == 0) {
                    printf("Please input matrix first.\n");
                    break;
                }
                if (matrix_transpose(&mat1,