//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix dimensions:\n");
    mat->rows = read_int("Rows (1-10): ", 1, MAX_SIZE);
    mat->cols = read_int("Columns (1-10): ", 1, MAX_SIZE);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("", -1000, 1000);
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

int matrix_determinant_recursive(const Matrix* mat, int size) {
    if (size == 1) {
        return mat->data[0][0];
    }
    
    if (size == 2) {
        long long result = (long long)mat->data[0][0] * mat->data[1][1] - 
                          (long long)mat->data[0][1] * mat->data[1][0];
        if (result > INT_MAX || result < INT_MIN) {
            printf("Integer overflow detected\n");
            return 0;
        }
        return (int)result;
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
        
        int sub_det = matrix_determinant_recursive(&submat, size - 1);
        
        long long term = (long long)sign * mat->data[0][col] * sub_det;
        if (term > 0) {
            if (det > INT_MAX - (int)term) {
                printf("Integer overflow detected\n");
                return 0;
            }
        } else if (term < 0) {
            if (det < INT_MIN - (int)term) {
                printf("Integer overflow detected\n");
                return 0;
            }
        }
        
        det += (int)term;
        sign = -sign;
    }
    
    return det;
}

int matrix_determinant(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        printf("Error: Determinant requires a square matrix\n");
        return 0;
    }
    
    if (mat->rows > MAX_SIZE) {
        printf("Error: Matrix too large\n");
        return 0;
    }
    
    return matrix_determinant_recursive(mat, mat->rows);
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
    
    read_matrix(&mat1);
    
    printf("\nOriginal Matrix:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix Determinant: ");
    int det = matrix_determinant(&mat1);
    printf("%d\n", det);
    
    if (mat1.rows == mat1.cols) {
        printf("\nMatrix Transpose:\n");
        matrix_transpose(&mat1, &mat2);
        print_matrix(&mat2);
    }
    
    return 0;
}