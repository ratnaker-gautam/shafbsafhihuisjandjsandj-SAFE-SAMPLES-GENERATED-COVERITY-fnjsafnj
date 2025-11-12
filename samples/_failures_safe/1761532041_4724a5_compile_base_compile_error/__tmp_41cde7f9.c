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
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("", INT_MIN, INT_MAX);
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

int recursive_determinant(const Matrix* mat, int size) {
    if (size == 1) {
        return mat->data[0][0];
    }
    
    if (size == 2) {
        int64_t product = (int64_t)mat->data[0][0] * mat->data[1][1] - 
                         (int64_t)mat->data[0][1] * mat->data[1][0];
        if (product > INT_MAX) return INT_MAX;
        if (product < INT_MIN) return INT_MIN;
        return (int)product;
    }
    
    int det = 0;
    Matrix submat;
    submat.rows = size - 1;
    submat.cols = size - 1;
    
    for (int col = 0; col < size; col++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                submat.data[sub_i][sub_j] = mat->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int64_t cofactor = (int64_t)mat->data[0][col] * recursive_determinant(&submat, size - 1);
        if (col % 2 == 1) {
            cofactor = -cofactor;
        }
        
        int64_t new_det = (int64_t)det + cofactor;
        if (new_det > INT_MAX) det = INT_MAX;
        else if (new_det < INT_MIN) det = INT_MIN;
        else det = (int)new_det;
    }
    
    return det;
}

int matrix_determinant(const Matrix* mat) {
    if (mat->rows != mat