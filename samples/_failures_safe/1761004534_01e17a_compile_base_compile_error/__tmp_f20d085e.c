//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows * a->cols; i++) {
        if ((*a_ptr > 0 && *b_ptr > INT_MAX - *a_ptr) ||
            (*a_ptr < 0 && *b_ptr < INT_MIN - *a_ptr)) {
            printf("Integer overflow detected in addition.\n");
            exit(1);
        }
        *res_ptr = *a_ptr + *b_ptr;
        a_ptr++;
        b_ptr++;
        res_ptr++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)*(a_ptr + i * a->cols + k) * 
                              (long)*(b_ptr + k * b->cols + j);
                
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    exit(1);
                }
                
                if ((sum > 0 && product > INT_MAX - sum) ||
                    (sum < 0 && product < INT_MIN - sum)) {
                    printf("Integer overflow detected in summation.\n");
                    exit(1);
                }
                
                sum += (int)product;
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    read_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    read_matrix(&mat2);
    
    printf("\nFirst Matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    print_matrix(&mat2);
    
    if (mat1.rows == mat2.rows && mat1.cols == mat2.cols) {
        result.rows = mat1.rows;
        result.cols = mat1.cols;
        add_matrices(&mat1, &mat2, &result);
        printf("\nMatrix Addition Result:\n");
        print_matrix(&result);
    } else {
        printf("\nMatrices cannot be added (dimensions don't match).\n");
    }
    
    if (mat1.cols == mat