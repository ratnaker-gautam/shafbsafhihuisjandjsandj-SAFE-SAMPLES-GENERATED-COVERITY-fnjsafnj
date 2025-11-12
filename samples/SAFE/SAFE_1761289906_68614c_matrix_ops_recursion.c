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
            printf("Invalid input. Please enter an integer.\n");
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
            char prompt[50];
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

int matrix_determinant_recursive(const Matrix* mat, int n) {
    if (n == 1) {
        return mat->data[0][0];
    }
    
    if (n == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < n; col++) {
        Matrix submat;
        submat.rows = n - 1;
        submat.cols = n - 1;
        
        int sub_i = 0;
        for (int i = 1; i < n; i++) {
            int sub_j = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                if (sub_i < submat.rows && sub_j < submat.cols) {
                    submat.data[sub_i][sub_j] = mat->data[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        int sub_det = matrix_determinant_recursive(&submat, n - 1);
        
        if (mat->data[0][col] != 0 && sub_det != 0) {
            if (__builtin_mul_overflow(mat->data[0][col], sub_det, &det)) {
                return 0;
            }
            if (__builtin_mul_overflow(sign, det, &det)) {
                return 0;
            }
        } else {
            det = 0;
        }
        
        det += sign * mat->data[0][col] * sub_det;
        sign = -sign;
    }
    
    return det;
}

int matrix_determinant(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        printf("Error: Determinant requires a square matrix.\n");
        return 0;
    }
    
    if (mat->rows > MAX_DIM) {
        printf("Error: Matrix too large for determinant calculation.\n");
        return 0;
    }
    
    return matrix_determinant_recursive(mat, mat->rows);
}

int main() {
    printf("Matrix Determinant Calculator\n");
    printf("=============================\n");
    
    Matrix mat;
    mat.rows = read_int("Enter number of rows (1-8): ", 1, MAX_DIM);
    mat.cols = read_int("Enter number of columns (1-8): ", 1, MAX_DIM);
    
    read_matrix(&mat);
    
    printf("\nEntered matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int det = matrix_determinant(&mat);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square. Cannot compute determinant.\n");
    }
    
    return 0;
}