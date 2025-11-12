//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            printf("Input error\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Please enter a valid integer between %d and %d\n", min, max);
    }
}

void read_matrix(Matrix* mat) {
    mat->rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    mat->cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -1000, 1000);
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
    
    if (row == col) {
        return recursive_trace(mat, row + 1, col + 1, sum);
    }
    
    return sum;
}

int matrix_trace(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    return recursive_trace(mat, 0, 0, 0);
}

void recursive_transpose(const Matrix* src, Matrix* dst, int row, int col) {
    if (row >= src->rows || col >= src->cols) {
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    
    if (col + 1 < src->cols) {
        recursive_transpose(src, dst, row, col + 1);
    } else if (row + 1 < src->rows) {
        recursive_transpose(src, dst, row + 1, 0);
    }
}

void matrix_transpose(const Matrix* src, Matrix* dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix mat1, mat2;
    
    printf("Matrix Operations using Recursion\n");
    printf("================================\n");
    
    printf("\nEnter first matrix:\n");
    read_matrix(&mat1);
    
    printf("\nOriginal Matrix:\n");
    print_matrix(&mat1);
    
    if (mat1.rows == mat1.cols) {
        int trace = matrix_trace(&mat1);
        printf("\nMatrix Trace: %d\n", trace);
    } else {
        printf("\nMatrix is not square - cannot compute trace\n");
    }
    
    printf("\nTransposed Matrix:\n");
    matrix_transpose(&mat1, &mat2);
    print_matrix(&mat2);
    
    return 0;
}