//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
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
    mat->rows = read_int("Enter number of rows (1-8): ", 1, MAX_DIM);
    mat->cols = read_int("Enter number of columns (1-8): ", 1, MAX_DIM);
    
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
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(const Matrix* mat, int row, int col, int sum) {
    if (row >= mat->rows || col >= mat->cols) {
        return sum;
    }
    
    if (row != col) {
        return recursive_trace(mat, row + 1, col + 1, sum);
    }
    
    int new_sum;
    if (sum > 0 && mat->data[row][col] > INT_MAX - sum) {
        new_sum = INT_MAX;
    } else if (sum < 0 && mat->data[row][col] < INT_MIN - sum) {
        new_sum = INT_MIN;
    } else {
        new_sum = sum + mat->data[row][col];
    }
    
    return recursive_trace(mat, row + 1, col + 1, new_sum);
}

int matrix_trace(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    return recursive_trace(mat, 0, 0, 0);
}

Matrix recursive_multiply(const Matrix* a, const Matrix* b, int row, int col, int k, Matrix* result) {
    if (row >= a->rows) {
        return *result;
    }
    
    if (col >= b->cols) {
        return recursive_multiply(a, b, row + 1, 0, 0, result);
    }
    
    if (k >= a->cols) {
        return recursive_multiply(a, b, row, col + 1, 0, result);
    }
    
    int product;
    if (a->data[row][k] > 0 && b->data[k][col] > 0) {
        if (a->data[row][k] > INT_MAX / b->data[k][col]) {
            product = INT_MAX;
        } else {
            product = a->data[row][k] * b->data[k][col];
        }
    } else if (a->data[row][k] < 0 && b->data[k][col] < 0) {
        if (a->data[row][k] < INT_MAX / b->data[k][col]) {
            product = INT_MAX;
        } else {
            product = a->data[row][k] * b->data[k][col];
        }
    } else {
        product = a->data[row][k] * b->data[k][col];
    }
    
    if (result->data[row][col] > 0 && product > INT_MAX - result->data[row][col]) {
        result->data[row][col] = INT_MAX;
    } else if (result->data[row][col] < 0 && product < INT_MIN - result->data[row][col]) {
        result->data[row][col] = INT_MIN;
    } else {
        result->data[row][col] += product;
    }
    
    return recursive_multiply(a, b, row, col, k + 1, result);
}

int can_multiply(const Matrix* a, const Matrix* b) {
    return a->cols == b->rows;
}

Matrix matrix_multiply(const Matrix* a, const Matrix* b) {
    Matrix result = {0};
    
    if (!can_multiply(a, b)) {
        result.rows = 0;
        result.cols = 0;
        return result;
    }
    
    result.rows = a->rows;
    result.cols = b->cols;
    
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j <