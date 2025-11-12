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

void init_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int("", INT_MIN, INT_MAX);
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

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrices must have same dimensions for addition.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows * a->cols; i++) {
        if ((*a_ptr > 0 && *b_ptr > INT_MAX - *a_ptr) ||
            (*a_ptr < 0 && *b_ptr < INT_MIN - *a_ptr)) {
            printf("Integer overflow detected in addition.\n");
            return;
        }
        *res_ptr = *a_ptr + *b_ptr;
        a_ptr++;
        b_ptr++;
        res_ptr++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Invalid dimensions for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    int* res_ptr = result->data;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a->data + i * a->cols + k);
                int b_val = *(b->data + k * b->cols + j);
                
                if (a_val != 0 && b_val != 0) {
                    if ((a_val > 0 && b_val > 0 && a_val > INT_MAX / b_val) ||
                        (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val) ||
                        (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                        (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val)) {
                        printf("Integer overflow detected in multiplication.\n");
                        return;
                    }
                }
                
                long long product = (long long)a_val * b_val;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                sum += (int)product;
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

void transpose_matrix(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    const int* src_ptr = mat->data;
    int* dest_ptr = result->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            *(dest_ptr + j * result->cols + i) = *(src_ptr + i * mat->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter number of columns (1-100): ",