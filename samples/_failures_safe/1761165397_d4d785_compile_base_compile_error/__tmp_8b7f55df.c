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
        
        if (temp > INT_MAX || temp < INT_MIN) {
            printf("Value out of range.\n");
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
    int total = mat->rows * mat->cols;
    
    for (int i = 0; i < total; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Element [%d,%d]: ", 
                i / mat->cols, i % mat->cols);
        
        int value = read_int(prompt, -1000, 1000);
        if (value == -1) {
            exit(EXIT_FAILURE);
        }
        
        *(ptr + i) = value;
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* ptr_a = a->data;
    const int* ptr_b = b->data;
    int* ptr_result = result->data;
    int total = a->rows * a->cols;
    
    for (int i = 0; i < total; i++) {
        if ((*(ptr_a + i) > 0 && *(ptr_b + i) > INT_MAX - *(ptr_a + i)) ||
            (*(ptr_a + i) < 0 && *(ptr_b + i) < INT_MIN - *(ptr_a + i))) {
            printf("Integer overflow detected in addition.\n");
            exit(EXIT_FAILURE);
        }
        *(ptr_result + i) = *(ptr_a + i) + *(ptr_b + i);
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* ptr_a = a->data;
    const int* ptr_b = b->data;
    int* ptr_result = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)*(ptr_a + i * a->cols + k) * 
                              (long)*(ptr_b + k * b->cols + j);
                
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    exit(EXIT_FAILURE);
                }
                
                if ((product > 0 && sum > INT_MAX - (int)product) ||
                    (product < 0 && sum < INT_MIN - (int)product)) {
                    printf("Integer overflow detected in summation.\n");
                    exit(EXIT_FAILURE);
                }
                
                sum += (int)product;
            }
            *(ptr_result + i * result->cols + j) = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix 1 dimensions:\n");
    mat1.rows = read_int("Rows: ", 1, MAX_DIM);
    mat1.cols = read_int("Columns: ", 1, MAX_DIM);
    
    if (!validate_dimension(mat1.rows) || !validate_dimension(mat1.cols)) {
        printf("Invalid matrix dimensions.\n");
        return EXIT_FAILURE;
    }
    
    read_matrix(&mat1);
    
    printf("Matrix 2 dimensions:\n");
    mat2.rows = read_int("Rows: ", 1, MAX_DIM);
    mat2.cols = read_int("Columns: ", 1, MAX_DIM);
    
    if (!validate_dimension(mat2.rows) || !validate_dimension(mat2.cols)) {
        printf("Invalid matrix dimensions.\n");
        return EXIT_FAILURE;
    }
    
    read_matrix(&