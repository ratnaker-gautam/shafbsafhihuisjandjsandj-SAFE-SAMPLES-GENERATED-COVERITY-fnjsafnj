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
    
    if (row == col) {
        sum += mat->data[row][col];
    }
    
    if (row == mat->rows - 1 && col == mat->cols - 1) {
        return sum;
    }
    
    if (col < mat->cols - 1) {
        return recursive_trace(mat, row, col + 1, sum);
    } else {
        return recursive_trace(mat, row + 1, 0, sum);
    }
}

int calculate_trace(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    return recursive_trace(mat, 0, 0, 0);
}

int recursive_sum(const Matrix* mat, int row, int col, int sum) {
    if (row >= mat->rows) {
        return sum;
    }
    
    if (col >= mat->cols) {
        return recursive_sum(mat, row + 1, 0, sum);
    }
    
    int new_sum;
    if (__builtin_add_overflow(sum, mat->data[row][col], &new_sum)) {
        return sum;
    }
    
    return recursive_sum(mat, row, col + 1, new_sum);
}

int calculate_sum(const Matrix* mat) {
    return recursive_sum(mat, 0, 0, 0);
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

void transpose_matrix(const Matrix* src, Matrix* dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal Matrix:\n");
    print_matrix(&mat);
    
    printf("\nMatrix Sum: %d\n", calculate_sum(&mat));
    
    int trace = calculate_trace(&mat);
    if (trace != 0) {
        printf("Matrix Trace: %d\n", trace);
    } else {
        printf("Matrix is not square - no trace calculated\n");
    }
    
    if (mat.rows <= MAX_DIM && mat.cols <= MAX_DIM) {
        transpose_matrix(&mat, &transposed);
        printf("\nTransposed Matrix:\n");
        print_matrix(&transposed);
    }
    
    return 0;
}