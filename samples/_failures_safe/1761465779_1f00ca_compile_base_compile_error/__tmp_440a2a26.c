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
    printf("Enter matrix elements row by row:\n");
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

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row = a->data + (i * a->cols);
            const int* b_col = b->data + j;
            
            for (int k = 0; k < a->cols; k++) {
                if (a_row[k] > 0 && b_col[k * b->cols] > 0) {
                    if (a_row[k] > INT_MAX / b_col[k * b->cols]) {
                        printf("Multiplication overflow detected.\n");
                        return;
                    }
                } else if (a_row[k] < 0 && b_col[k * b->cols] < 0) {
                    if (a_row[k] < INT_MAX / b_col[k * b->cols]) {
                        printf("Multiplication overflow detected.\n");
                        return;
                    }
                }
                
                long product = (long)a_row[k] * (long)b_col[k * b->cols];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Multiplication overflow detected.\n");
                    return;
                }
                
                if (sum > 0 && product > INT_MAX - sum) {
                    printf("Addition overflow detected.\n");
                    return;
                } else if (sum < 0 && product < INT_MIN - sum) {
                    printf("Addition overflow detected.\n");
                    return;
                }
                
                sum += (int)product;
            }
            
            *res_ptr = sum;
            res_ptr++;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("==============================================\n\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    
    if (mat1.rows == -1 || mat1.cols == -1) {
        printf("Error reading matrix dimensions.\n");
        return 1;
    }
    
    read_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    
    if (mat2.rows == -1 || mat2.cols == -1) {
        printf("Error reading matrix dimensions.\n");
        return 1;
    }
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Cannot multiply matrices. Columns of first matrix must equal rows of second matrix.\n");
        return 1;
    }
    
    read_matrix(&mat2);
    
    result.rows = mat1.rows;
    result.cols = mat2.cols;
    
    printf("\nFirst Matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    print_matrix(&mat2);
    
    printf("\nResult Matrix:\n");
    multiply_matrices