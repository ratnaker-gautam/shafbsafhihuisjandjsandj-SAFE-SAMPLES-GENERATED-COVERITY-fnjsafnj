//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_INVALID
};

struct Matrix {
    int rows;
    int cols;
    int data[10][10];
};

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(struct Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows (1-10): ", 1, 10);
    mat->cols = read_int("Columns (1-10): ", 1, 10);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const struct Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

enum Operation get_operation() {
    printf("\nAvailable operations:\n");
    printf("0 - Add matrices\n");
    printf("1 - Subtract matrices\n");
    printf("2 - Multiply matrices\n");
    printf("3 - Transpose matrix\n");
    
    int choice = read_int("Select operation (0-3): ", 0, 3);
    return (enum Operation)choice;
}

int matrix_add(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Error: Matrices must have same dimensions for addition.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                printf("Error: Integer overflow in addition.\n");
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    
    return 1;
}

int matrix_subtract(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Error: Matrices must have same dimensions for subtraction.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((b->data[i][j] > 0 && a->data[i][j] < INT_MIN + b->data[i][j]) ||
                (b->data[i][j] < 0 && a->data[i][j] > INT_MAX + b->data[i][j])) {
                printf("Error: Integer overflow in subtraction.\n");
                return 0;
            }
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    
    return 1;
}

int matrix_multiply(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    if (a->cols != b->rows) {
        printf("Error: Matrix A columns must equal Matrix B rows for multiplication.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {