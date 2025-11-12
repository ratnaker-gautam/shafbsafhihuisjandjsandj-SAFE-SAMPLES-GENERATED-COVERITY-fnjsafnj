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
        
        if (temp < min || temp > max || temp > INT_MAX) {
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
    int total = mat->rows * mat->cols;
    
    for (int i = 0; i < total; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Element [%d,%d]: ", 
                 i / mat->cols, i % mat->cols);
        
        int value = read_int(prompt, -1000, 1000);
        if (value == -1) {
            exit(1);
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

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row = a->data + i * a->cols;
            const int* b_col = b->data + j;
            
            for (int k = 0; k < a->cols; k++) {
                int product = *(a_row + k) * *(b_col + k * b->cols);
                
                if (*(a_row + k) > 0 && *(b_col + k * b->cols) > 0) {
                    if (product > INT_MAX - sum) {
                        printf("Integer overflow detected!\n");
                        exit(1);
                    }
                } else if (*(a_row + k) < 0 && *(b_col + k * b->cols) < 0) {
                    if (product > INT_MAX - sum) {
                        printf("Integer overflow detected!\n");
                        exit(1);
                    }
                } else if (*(a_row + k) > 0 && *(b_col + k * b->cols) < 0) {
                    if (product < INT_MIN - sum) {
                        printf("Integer overflow detected!\n");
                        exit(1);
                    }
                } else if (*(a_row + k) < 0 && *(b_col + k * b->cols) > 0) {
                    if (product < INT_MIN - sum) {
                        printf("Integer overflow detected!\n");
                        exit(1);
                    }
                }
                
                sum += product;
            }
            
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication Calculator\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter rows: ", 1, MAX_DIM);
    if (mat1.rows == -1) return 1;
    mat1.cols = read_int("Enter columns: ", 1, MAX_DIM);
    if (mat1.cols == -1) return 1;
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter rows: ", 1, MAX_DIM);
    if (mat2.rows == -1) return 1;
    mat2.cols = read_int("Enter columns: ", 1, MAX_DIM);
    if (mat2.cols == -1) return 1;
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        printf("First matrix columns (%d) must equal second matrix rows (%d).\n", 
               mat1.cols, mat2.rows);
        return 1;
    }
    
    if (mat1.rows > MAX_DIM || mat1.cols > MAX_DIM || 
        mat2.rows > MAX_DIM || mat2.cols > MAX_DIM) {