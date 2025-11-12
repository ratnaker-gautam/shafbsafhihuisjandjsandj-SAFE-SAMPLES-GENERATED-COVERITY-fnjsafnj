//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        if (temp > INT_MAX || temp < INT_MIN) {
            printf("Value out of range.\n");
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void init_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void fill_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
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

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    init_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    return 0;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    
    return 1;
}

int transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
    
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    fill_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    fill_matrix(&mat2);
    
    printf("\nFirst Matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        printf("Result:\n");
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices: dimension mismatch or overflow.\n");
    }
    
    printf("\nTranspose of First Matrix:\n");
    if (transpose_matrix(&mat1, &result)) {
        print_matrix(&result);
    }
    
    printf("\nTranspose of Second Matrix:\n");
    if (transpose_matrix(&mat2, &result)) {
        print_matrix(&result);
    }
    
    return 0;
}