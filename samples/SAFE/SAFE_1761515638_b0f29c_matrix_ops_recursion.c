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
        
        if (temp >= min && temp <= max && temp <= INT_MAX) {
            value = (int)temp;
            break;
        }
    }
    return value;
}

void read_matrix(Matrix* mat) {
    mat->rows = read_int("Enter number of rows (1-8): ", 1, MAX_SIZE);
    mat->cols = read_int("Enter number of columns (1-8): ", 1, MAX_SIZE);
    
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

int matrix_trace_recursive(const Matrix* mat, int row) {
    if (row >= mat->rows || row >= mat->cols) {
        return 0;
    }
    
    int sum = mat->data[row][row];
    int next_sum = matrix_trace_recursive(mat, row + 1);
    
    if ((sum > 0 && next_sum > INT_MAX - sum) || 
        (sum < 0 && next_sum < INT_MIN - sum)) {
        return 0;
    }
    
    return sum + next_sum;
}

int matrix_trace(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    return matrix_trace_recursive(mat, 0);
}

void matrix_transpose_recursive(const Matrix* src, Matrix* dst, int row, int col) {
    if (row >= src->rows) {
        return;
    }
    
    if (col >= src->cols) {
        matrix_transpose_recursive(src, dst, row + 1, 0);
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    matrix_transpose_recursive(src, dst, row, col + 1);
}

void matrix_transpose(const Matrix* src, Matrix* dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    matrix_transpose_recursive(src, dst, 0, 0);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations using Recursion\n");
    printf("=================================\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal Matrix:\n");
    print_matrix(&mat);
    
    int trace = matrix_trace(&mat);
    if (mat.rows == mat.cols) {
        printf("\nTrace of matrix: %d\n", trace);
    } else {
        printf("\nMatrix is not square, cannot compute trace\n");
    }
    
    matrix_transpose(&mat, &transposed);
    printf("\nTransposed Matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}