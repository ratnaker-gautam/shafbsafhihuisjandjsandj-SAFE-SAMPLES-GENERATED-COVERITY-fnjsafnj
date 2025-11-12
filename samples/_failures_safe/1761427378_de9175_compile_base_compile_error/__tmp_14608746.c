//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
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
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements row by row:\n");
    
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int value = read_int(prompt, INT_MIN, INT_MAX);
            if (value == -1) {
                printf("Input error.\n");
                exit(1);
            }
            *(ptr++) = value;
        }
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr++));
        }
        printf("\n");
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        exit(1);
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    int* res_ptr = result->data;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row = a->data + (i * a->cols);
            const int* b_col = b->data + j;
            
            for (int k = 0; k < a->cols; k++) {
                if ((sum > 0 && *a_row > INT_MAX - sum) || 
                    (sum < 0 && *a_row < INT_MIN - sum)) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                sum += *(a_row++) * (*b_col);
                b_col += b->cols;
            }
            
            *(res_ptr++) = sum;
        }
    }
}

void transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    int* res_ptr = result->data;
    for (int j = 0; j < input->cols; j++) {
        const int* col_ptr = input->data + j;
        for (int i = 0; i < input->rows; i++) {
            *(res_ptr++) = *col_ptr;
            col_ptr += input->cols;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for first matrix (1-100): ", 1, MAX_DIM);
    if (mat1.rows == -1) return 1;
    
    mat1.cols = read_int("Enter columns for first matrix (1-100): ", 1, MAX_DIM);
    if (mat1.cols == -1) return 1;
    
    read_matrix(&mat1);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nChoose operation:\n");
    printf("1. Matrix multiplication\n");
    printf("2. Matrix transpose\n");
    
    int choice = read_int("Enter choice (1-2): ", 1, 2);
    if (choice == -1) return 1;
    
    if (choice == 1) {
        mat2.rows = read_int("Enter rows for second matrix (1-100): ", 1, MAX_DIM);
        if (mat2.rows == -1) return 1;
        
        mat2.cols = read_int("Enter columns for second matrix (1-100): ", 1, MAX_DIM);
        if (mat2.cols == -1) return 1;
        
        read_matrix(&mat2);
        
        printf("\nSecond matrix:\n");
        print_matrix(&mat2);
        
        multiply_matrices(&mat1, &mat2, &result);
        
        printf("\nResult of multiplication