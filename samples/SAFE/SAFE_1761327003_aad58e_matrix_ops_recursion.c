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
        
        if (temp >= min && temp <= max && temp <= INT_MAX && temp >= INT_MIN) {
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
            snprintf(prompt, sizeof(prompt), "[%d][%d]: ", i, j);
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

int recursive_determinant(const Matrix* mat, int size) {
    if (size == 1) {
        return mat->data[0][0];
    }
    
    if (size == 2) {
        int64_t det = (int64_t)mat->data[0][0] * mat->data[1][1] - 
                     (int64_t)mat->data[0][1] * mat->data[1][0];
        if (det > INT_MAX || det < INT_MIN) {
            return 0;
        }
        return (int)det;
    }
    
    int determinant = 0;
    
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
        
        int64_t cofactor = (int64_t)mat->data[0][col] * recursive_determinant(&submat, size - 1);
        if (col % 2 == 1) {
            cofactor = -cofactor;
        }
        
        int64_t new_det = (int64_t)determinant + cofactor;
        if (new_det > INT_MAX || new_det < INT_MIN) {
            return 0;
        }
        determinant = (int)new_det;
    }
    
    return determinant;
}

int matrix_determinant(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    return recursive_determinant(mat, mat->rows);
}

int main() {
    Matrix mat;
    
    printf("Matrix Determinant Calculator\n");
    printf("=============================\n");
    
    read_matrix(&mat);
    
    printf("\nInput matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int det = matrix_determinant(&mat);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square. Cannot compute determinant.\n");
    }
    
    return 0;
}