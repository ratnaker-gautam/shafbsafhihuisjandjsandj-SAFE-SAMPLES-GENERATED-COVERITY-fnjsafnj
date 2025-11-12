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
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    int total = mat->rows * mat->cols;
    
    for (int i = 0; i < total; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Element [%d]: ", i);
        int value = read_int(prompt, INT_MIN, INT_MAX);
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

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += *(a->data + i * a->cols + k) * *(b->data + k * b->cols + j);
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    printf("First matrix:\n");
    mat1.rows = read_int("Enter rows: ", 1, MAX_DIM);
    if (mat1.rows == -1) return EXIT_FAILURE;
    mat1.cols = read_int("Enter columns: ", 1, MAX_DIM);
    if (mat1.cols == -1) return EXIT_FAILURE;
    
    printf("\nSecond matrix:\n");
    mat2.rows = read_int("Enter rows: ", 1, MAX_DIM);
    if (mat2.rows == -1) return EXIT_FAILURE;
    mat2.cols = read_int("Enter columns: ", 1, MAX_DIM);
    if (mat2.cols == -1) return EXIT_FAILURE;
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Cannot multiply matrices. Columns of first must equal rows of second.\n");
        return EXIT_FAILURE;
    }
    
    if (mat1.rows * mat2.cols > MAX_DIM * MAX_DIM) {
        printf("Error: Result matrix would exceed maximum size.\n");
        return EXIT_FAILURE;
    }
    
    result.rows = mat1.rows;
    result.cols = mat2.cols;
    
    printf("\n");
    read_matrix(&mat1);
    printf("\n");
    read_matrix(&mat2);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    multiply_matrices(&mat1, &mat2, &result);
    
    printf("\nResult matrix:\n");
    print_matrix(&result);
    
    return EXIT_SUCCESS;
}