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
            *ptr = read_int(prompt, INT_MIN, INT_MAX);
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

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* ptr_a = a->data;
    const int* ptr_b = b->data;
    int* ptr_result = result->data;
    
    for (int i = 0; i < a->rows * a->cols; i++) {
        if ((*ptr_a > 0 && *ptr_b > INT_MAX - *ptr_a) ||
            (*ptr_a < 0 && *ptr_b < INT_MIN - *ptr_a)) {
            printf("Integer overflow detected in addition.\n");
            exit(1);
        }
        *ptr_result = *ptr_a + *ptr_b;
        ptr_a++;
        ptr_b++;
        ptr_result++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int* ptr_result = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            
            for (int k = 0; k < a->cols; k++) {
                const int* elem_a = a->data + i * a->cols + k;
                const int* elem_b = b->data + k * b->cols + j;
                
                if (*elem_a != 0 && *elem_b != 0) {
                    if ((*elem_a > 0 && *elem_b > 0 && *elem_a > INT_MAX / *elem_b) ||
                        (*elem_a < 0 && *elem_b < 0 && *elem_a < INT_MAX / *elem_b) ||
                        (*elem_a > 0 && *elem_b < 0 && *elem_b < INT_MIN / *elem_a) ||
                        (*elem_a < 0 && *elem_b > 0 && *elem_a < INT_MIN / *elem_b)) {
                        printf("Integer overflow detected in multiplication.\n");
                        exit(1);
                    }
                }
                
                long product = (long)(*elem_a) * (long)(*elem_b);
                
                if (sum > 0 && product > INT_MAX - sum) {
                    printf("Integer overflow detected in summation.\n");
                    exit(1);
                }
                if (sum < 0 && product < INT_MIN - sum) {
                    printf("Integer overflow detected in summation.\n");
                    exit(1);
                }
                
                sum += (int)product;
            }
            
            *ptr_result = sum;
            ptr_result++;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
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