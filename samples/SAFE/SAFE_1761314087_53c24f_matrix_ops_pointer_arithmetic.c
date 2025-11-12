//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
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

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void transpose_matrix(int* src, int* dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_rows, int b_cols) {
    if (a_cols != b_rows) {
        return 0;
    }
    
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
    
    return 1;
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("1. Transpose matrix\n");
    printf("2. Multiply matrices\n");
    
    choice = read_int("Enter choice (1-2): ", 1, 2);
    
    if (choice == 1) {
        rows1 = read_int("Enter number of rows: ", 1, MAX_DIM);
        cols1 = read_int("Enter number of columns: ", 1, MAX_DIM);
        
        int matrix1[MAX_DIM * MAX_DIM];
        int result[MAX_DIM * MAX_DIM];
        
        printf("Enter matrix elements:\n");
        fill_matrix(matrix1, rows1, cols1);
        
        transpose_matrix(matrix1, result, rows1, cols1);
        
        printf("Original ");
        print_matrix(matrix1, rows1, cols1);
        
        printf("Transposed ");
        print_matrix(result, cols1, rows1);
        
    } else {
        rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
        cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
        rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
        cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
        
        if (cols1 != rows2) {
            printf("Matrix multiplication not possible: columns of first must equal rows of second.\n");
            return 1;
        }
        
        int matrix1[MAX_DIM * MAX_DIM];
        int matrix2[MAX_DIM * MAX_DIM];
        int result[MAX_DIM * MAX_DIM];
        
        printf("Enter elements for matrix 1:\n");
        fill_matrix(matrix1, rows1, cols1);
        
        printf("Enter elements for matrix 2:\n");
        fill_matrix(matrix2, rows2, cols2);
        
        if (multiply_matrices(matrix1, matrix2, result, rows1, cols1, rows2, cols2)) {
            printf("Matrix 1:\n");
            print_matrix(matrix1, rows1, cols1);
            
            printf("Matrix 2:\n");
            print_matrix(matrix2, rows2, cols2);
            
            printf("Product:\n");
            print_matrix(result, rows1, cols2);
        } else {
            printf("Matrix multiplication failed.\n");
        }
    }
    
    return 0;
}