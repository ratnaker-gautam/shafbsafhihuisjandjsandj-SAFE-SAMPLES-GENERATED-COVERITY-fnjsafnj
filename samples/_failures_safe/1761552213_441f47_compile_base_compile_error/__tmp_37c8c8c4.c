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

int recursive_determinant(const Matrix* mat, int size) {
    if (size == 1) {
        return mat->data[0][0];
    }
    
    if (size == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
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
                if (j != col) {
                    submat.data[sub_i][sub_j] = mat->data[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        int sign = (col % 2 == 0) ? 1 : -1;
        int sub_det = recursive_determinant(&submat, size - 1);
        
        if (mat->data[0][col] > 0 && sub_det > 0 && mat->data[0][col] > INT_MAX / sub_det) {
            return 0;
        }
        if (mat->data[0][col] < 0 && sub_det < 0 && mat->data[0][col] < INT_MAX / sub_det) {
            return 0;
        }
        if (mat->data[0][col] > 0 && sub_det < 0 && mat->data[0][col] < INT_MIN / sub_det) {
            return 0;
        }
        if (mat->data[0][col] < 0 && sub_det > 0 && mat->data[0][col] < INT_MIN / sub_det) {
            return 0;
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
        printf("Matrix must be square for determinant calculation.\n");
        return 0;
    }
    
    if (mat->rows > MAX_SIZE) {
        printf("Matrix size too large for determinant calculation.\n");
        return 0;
    }
    
    return recursive_determinant(mat, mat->rows);
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter number of rows (1-8): ", 1, MAX_SIZE);
    mat1.cols = read_int("Enter number of columns (1-8): ", 1, MAX_SIZE);
    
    read_matrix(&mat1);
    
    printf("\nOriginal Matrix:\n");
    print_matrix(&mat1);
    
    if (mat1.rows == mat1.cols) {
        int det = matrix_determinant(&mat1);
        printf("\nDeterminant: %d