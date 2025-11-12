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
            printf("Invalid input. Please enter a valid integer.\n");
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
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int* res_ptr = result->data;
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a_ptr + i * a->cols + k);
                int b_val = *(b_ptr + k * b->cols + j);
                if ((a_val > 0 && b_val > 0 && a_val > INT_MAX / b_val) ||
                    (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val) ||
                    (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                    (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val)) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                sum += a_val * b_val;
                if ((sum ^ a_val) < 0 && (sum ^ b_val) < 0 && a_val != 0 && b_val != 0) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    const int* src_ptr = src->data;
    int* dest_ptr = dest->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
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
    printf("1. Transpose\n");
    printf("2. Multiply with another matrix\n");
    
    int choice = read_int("Enter choice (1-2): ", 1, 2);
    if (choice == -1) return 1;
    
    if (choice == 1) {
        result.rows = mat1.cols;
        result.cols = mat1.rows;
        transpose_matrix(&mat1, &result);
        printf("\nTransposed matrix:\n");
        print_matrix(&result);
    } else {
        mat2.rows = read_int("Enter rows for second matrix (1-100): ", 1, MAX_DIM);