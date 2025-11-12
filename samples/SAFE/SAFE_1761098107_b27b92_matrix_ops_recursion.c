//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_determinant(const Matrix* mat, int size) {
    if (size == 1) {
        return mat->data[0][0];
    }
    
    if (size == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        Matrix submat;
        submat.rows = size - 1;
        submat.cols = size - 1;
        
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
        
        int sub_det = recursive_determinant(&submat, size - 1);
        
        if (mat->data[0][col] != 0 && sub_det != 0) {
            if (__builtin_mul_overflow(mat->data[0][col], sub_det, &sub_det)) {
                printf("Warning: Integer overflow in determinant calculation\n");
            }
        }
        
        int term = mat->data[0][col] * sub_det;
        if (sign > 0) {
            if (__builtin_add_overflow(det, term, &det)) {
                printf("Warning: Integer overflow in determinant calculation\n");
            }
        } else {
            if (__builtin_sub_overflow(det, term, &det)) {
                printf("Warning: Integer overflow in determinant calculation\n");
            }
        }
        
        sign = -sign;
    }
    
    return det;
}

int main() {
    printf("Matrix Determinant Calculator\n");
    printf("=============================\n");
    
    Matrix mat;
    mat.rows = read_int("Enter number of rows (1-8): ", 1, MAX_DIM);
    mat.cols = read_int("Enter number of columns (1-8): ", 1, MAX_DIM);
    
    if (mat.rows != mat.cols) {
        printf("Error: Determinant is only defined for square matrices.\n");
        return 1;
    }
    
    read_matrix(&mat);
    
    printf("\nInput matrix:\n");
    print_matrix(&mat);
    
    int det = recursive_determinant(&mat, mat.rows);
    printf("\nDeterminant: %d\n", det);
    
    return 0;
}