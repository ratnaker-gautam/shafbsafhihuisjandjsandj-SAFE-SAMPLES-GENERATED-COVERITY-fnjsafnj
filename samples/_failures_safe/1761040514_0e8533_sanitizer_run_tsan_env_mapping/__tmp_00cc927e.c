//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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
    Matrix submat;
    submat.rows = n - 1;
    submat.cols = n - 1;
    
    for (int col = 0; col < n; col++) {
        int sub_i = 0;
        for (int i = 1; i < n; i++) {
            int sub_j = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                submat.data[sub_i][sub_j] = mat->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int sign = (col % 2 == 0) ? 1 : -1;
        int sub_det = matrix_determinant_recursive(&submat, n - 1);
        
        if (sign > 0) {
            if (mat->data[0][col] > 0 && sub_det > INT_MAX / mat->data[0][col]) {
                return 0;
            }
            if (mat->data[0][col] < 0 && sub_det < INT_MIN / mat->data[0][col]) {
                return 0;
            }
        } else {
            if (mat->data[0][col] > 0 && sub_det < INT_MIN / mat->data[0][col]) {
                return 0;
            }
            if (mat->data[0][col] < 0 && sub_det > INT_MAX / -mat->data[0][col]) {
                return 0;
            }
        }
        
        int term = sign * mat->data[0][col] * sub_det;
        if ((term > 0 && det > INT_MAX - term) || (term < 0 && det < INT_MIN - term)) {
            return 0;
        }
        det += term;
    }
    
    return det;
}

int matrix_determinant(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        printf("Determinant is only defined for square matrices.\n");
        return 0;
    }
    return matrix_determinant_recursive(mat, mat->rows);
}

int main() {
    Matrix mat;
    
    printf("Matrix Determinant Calculator\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    mat.rows = read_int("Enter number of rows: ", 1, MAX_SIZE);
    mat.cols = read_int("Enter number of columns: ", 1, MAX_SIZE);
    
    read_matrix(&mat);
    
    printf("\nYour matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int det = matrix_determinant(&mat);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nCannot compute determinant for non-square matrix.\n");
    }
    
    return 0;
}