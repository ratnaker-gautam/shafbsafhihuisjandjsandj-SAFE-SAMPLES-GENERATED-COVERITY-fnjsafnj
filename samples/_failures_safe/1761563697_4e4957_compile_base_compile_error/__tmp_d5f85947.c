//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[32];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
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
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_trace_recursive(const Matrix* mat, int row) {
    if (row >= mat->rows || row >= mat->cols) {
        return 0;
    }
    
    int current = mat->data[row][row];
    int rest = matrix_trace_recursive(mat, row + 1);
    
    if ((current > 0 && rest > INT_MAX - current) ||
        (current < 0 && rest < INT_MIN - current)) {
        return 0;
    }
    
    return current + rest;
}

int matrix_trace(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    return matrix_trace_recursive(mat, 0);
}

Matrix matrix_multiply_recursive(const Matrix* a, const Matrix* b, int i, int j, int k) {
    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;
    
    if (i >= a->rows) {
        return result;
    }
    
    if (j >= b->cols) {
        Matrix next_row = matrix_multiply_recursive(a, b, i + 1, 0, 0);
        for (int x = 0; x < a->rows; x++) {
            for (int y = 0; y < b->cols; y++) {
                result.data[x][y] = next_row.data[x][y];
            }
        }
        return result;
    }
    
    if (k >= a->cols) {
        Matrix next_col = matrix_multiply_recursive(a, b, i, j + 1, 0);
        for (int x = 0; x < a->rows; x++) {
            for (int y = 0; y < b->cols; y++) {
                result.data[x][y] = next_col.data[x][y];
            }
        }
        result.data[i][j] = 0;
        return result;
    }
    
    long long product = (long long)a->data[i][k] * b->data[k][j];
    Matrix partial = matrix_multiply_recursive(a, b, i, j, k + 1);
    
    for (int x = 0; x < a->rows; x++) {
        for (int y = 0; y < b->cols; y++) {
            result.data[x][y] = partial.data[x][y];
        }
    }
    
    if (product > 0 && result.data[i][j] > INT_MAX - product) {
        result.data[i][j] = INT_MAX;
    } else if (product < 0 && result.data[i][j] < INT_MIN - product) {
        result.data[i][j] = INT_MIN;
    } else {
        result.data[i][j] += (int)product;
    }
    
    return result;
}

Matrix matrix_multiply(const Matrix* a, const Matrix* b) {
    Matrix result;
    result.rows = a->rows;
    result.cols = b->cols;
    
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        result.rows = result.cols = 0;
        return result;
    }
    
    if (a->rows > MAX_SIZE || b->cols > MAX_SIZE) {
        printf("Result matrix would exceed maximum size.\n");
        result.rows = result.cols = 0;
        return result;
    }
    
    return matrix_multiply_recursive(a, b, 0, 0, 0);
}

int main() {
    Matrix a, b;
    
    printf("Matrix Operations Program\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    a.rows = read_int("Enter rows for matrix A (1-8): ", 1, MAX_SIZE);
    a.cols = read_int("Enter columns for matrix A (1-8