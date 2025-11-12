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
    char buffer[32];
    
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
    mat->rows = read_int("Enter rows (1-8): ", 1, MAX_DIM);
    mat->cols = read_int("Enter cols (1-8): ", 1, MAX_DIM);
    
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

void recursive_transpose(const Matrix* src, Matrix* dst, int row, int col) {
    if (row >= src->rows) {
        return;
    }
    
    if (col >= src->cols) {
        recursive_transpose(src, dst, row + 1, 0);
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    recursive_transpose(src, dst, row, col + 1);
}

void matrix_transpose(const Matrix* src, Matrix* dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal Matrix:\n");
    print_matrix(&mat);
    
    int trace = matrix_trace(&mat);
    if (mat.rows == mat.cols) {
        printf("\nMatrix Trace: %d\n", trace);
    } else {
        printf("\nMatrix is not square - no trace computed\n");
    }
    
    matrix_transpose(&mat, &transposed);
    printf("\nTransposed Matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}