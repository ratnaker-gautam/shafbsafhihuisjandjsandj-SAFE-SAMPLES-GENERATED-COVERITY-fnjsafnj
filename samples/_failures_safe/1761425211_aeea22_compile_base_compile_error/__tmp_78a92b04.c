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

void init_matrix(Matrix* mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void input_matrix(Matrix* mat) {
    printf("Enter matrix elements row by row:\n");
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int value = read_int(prompt, INT_MIN, INT_MAX);
            if (value == -1) exit(1);
            *(ptr + i * mat->cols + j) = value;
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

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a_ptr + i * a->cols + k);
                int b_val = *(b_ptr + k * b->cols + j);
                if ((a_val > 0 && b_val > INT_MAX / a_val) || 
                    (a_val < 0 && b_val < INT_MAX / a_val)) {
                    return 0;
                }
                sum += a_val * b_val;
                if ((sum > 0 && a_val * b_val > INT_MAX - sum) ||
                    (sum < 0 && a_val * b_val < INT_MIN - sum)) {
                    return 0;
                }
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    const int* src_ptr = src->data;
    int* dest_ptr = dest->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
        }
    }
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for first matrix (1-100): ", 1, MAX_DIM);
    if (rows1 == -1) return 1;
    
    int cols1 = read_int("Enter columns for first matrix (1-100): ", 1, MAX_DIM);
    if (cols1 == -1) return 1;
    
    Matrix mat1;
    init_matrix(&mat1, rows1, cols1);
    input_matrix(&mat1);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    int choice = read_int("\nChoose operation:\n1. Matrix multiplication\n2. Matrix transpose\nChoice: ", 1, 2);
    if (choice == -1) return 1;
    
    if (choice == 1) {
        int rows2 = read_int("Enter rows for second matrix (1-100): ", 1, MAX_DIM);
        if (rows2 == -1) return 1;
        
        int cols2 = read_int("Enter columns for second matrix (1-100): ", 1, MAX