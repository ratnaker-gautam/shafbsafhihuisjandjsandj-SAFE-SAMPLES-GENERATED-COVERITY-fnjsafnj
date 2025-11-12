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
    printf("Enter matrix elements row by row:\n");
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(mat->data + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += (*(a->data + i * a->cols + k)) * (*(b->data + k * b->cols + j));
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest->data + j * dest->cols + i) = *(src->data + i * src->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    
    choice = read_int("Choose operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        printf("Matrix A:\n");
        mat1.rows = read_int("Enter rows for matrix A: ", 1, MAX_DIM);
        mat1.cols = read_int("Enter columns for matrix A: ", 1, MAX_DIM);
        read_matrix(&mat1);
        
        printf("Matrix B:\n");
        mat2.rows = read_int("Enter rows for matrix B: ", 1, MAX_DIM);
        mat2.cols = read_int("Enter columns for matrix B: ", 1, MAX_DIM);
        
        if (mat1.cols != mat2.rows) {
            printf("Error: Matrix A columns must equal Matrix B rows for multiplication.\n");
            return 1;
        }
        
        read_matrix(&mat2);
        
        result.rows = mat1.rows;
        result.cols = mat2.cols;
        multiply_matrices(&mat1, &mat2, &result);
        
        printf("Matrix A:\n");
        print_matrix(&mat1);
        printf("Matrix B:\n");
        print_matrix(&mat2);
        printf("Result (A * B):\n");
        print_matrix(&result);
        
    } else if (choice == 2) {
        printf("Matrix to transpose:\n");
        mat1.rows = read_int("Enter rows: ", 1, MAX_DIM);
        mat1.cols = read_int("Enter columns: ", 1, MAX_DIM);
        read_matrix(&mat1);
        
        result.rows = mat1.cols;
        result.cols = mat1.rows;
        transpose_matrix(&mat1, &result);
        
        printf("Original matrix:\n");
        print_matrix(&mat1);
        printf("Transposed matrix:\n");
        print_matrix(&result);
    }
    
    return 0;
}