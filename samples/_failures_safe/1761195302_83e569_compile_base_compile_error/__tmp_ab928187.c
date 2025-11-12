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

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

void init_matrix(Matrix* mat) {
    printf("Initializing matrix %dx%d:\n", mat->rows, mat->cols);
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *ptr = read_int("", INT_MIN, INT_MAX);
            ptr++;
        }
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *ptr);
            ptr++;
        }
        printf("\n");
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
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
                if (sum > 0 && *a_row > 0 && *b_col > INT_MAX - sum) {
                    printf("Integer overflow detected.\n");
                    return;
                }
                if (sum < 0 && *a_row < 0 && *b_col < INT_MIN - sum) {
                    printf("Integer overflow detected.\n");
                    return;
                }
                
                sum += (*a_row) * (*b_col);
                a_row++;
                b_col += b->cols;
            }
            
            *res_ptr = sum;
            res_ptr++;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    int* dest_ptr = dest->data;
    const int* src_ptr = src->data;
    
    for (int j = 0; j < src->cols; j++) {
        const int* col_ptr = src_ptr + j;
        for (int i = 0; i < src->rows; i++) {
            *dest_ptr = *col_ptr;
            dest_ptr++;
            col_ptr += src->cols;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.rows == -1) return 1;
    
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.cols == -1) return 1;
    
    init_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nChoose operation:\n");
    printf("1. Transpose\n");
    printf("2. Multiply with another matrix\n");
    
    int choice = read_int("Enter choice (1-2): ", 1, 2);
    if (choice == -1) return 1;
    
    if (choice == 1) {
        transpose_matrix(&mat1, &result);
        printf("\nTranspose of matrix 1:\n");
        print_matrix(&result);
    } else {
        mat2.rows = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
        if (mat2.rows == -1) return 1;
        
        mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
        if (mat2.cols == -1) return 1;
        
        if (mat1.cols != mat2.rows) {
            printf("Matrix 1 columns must equal