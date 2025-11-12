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
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value >= min && value <= max) return value;
        printf("Value must be between %d and %d.\n", min, max);
    }
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

int multiply_matrices(int* a, int a_rows, int a_cols, int* b, int b_rows, int b_cols, int* result) {
    if (a_cols != b_rows) return 0;
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
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("Matrix A dimensions:\n");
    rows1 = read_int("Rows: ", 1, MAX_DIM);
    cols1 = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Matrix B dimensions:\n");
    rows2 = read_int("Rows: ", 1, MAX_DIM);
    cols2 = read_int("Columns: ", 1, MAX_DIM);
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    
    printf("Enter elements for Matrix A:\n");
    fill_matrix(matrix_a, rows1, cols1);
    
    printf("Enter elements for Matrix B:\n");
    fill_matrix(matrix_b, rows2, cols2);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows1, cols1);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows2, cols2);
    
    printf("\nTranspose of A:\n");
    int transpose_a[MAX_DIM * MAX_DIM];
    transpose_matrix(matrix_a, transpose_a, rows1, cols1);
    print_matrix(transpose_a, cols1, rows1);
    
    printf("\nTranspose of B:\n");
    int transpose_b[MAX_DIM * MAX_DIM];
    transpose_matrix(matrix_b, transpose_b, rows2, cols2);
    print_matrix(transpose_b, cols2, rows2);
    
    printf("\nMatrix Multiplication A x B:\n");
    int product[MAX_DIM * MAX_DIM];
    if (multiply_matrices(matrix_a, rows1, cols1, matrix_b, rows2, cols2, product)) {
        print_matrix(product, rows1, cols2);
    } else {
        printf("Cannot multiply: inner dimensions don't match (%d != %d)\n", cols1, rows2);
    }
    
    printf("\nMatrix Multiplication B x A:\n");
    int product2[MAX_DIM * MAX_DIM];
    if (multiply_matrices(matrix_b, rows2, cols2, matrix_a, rows1, cols1, product2)) {
        print_matrix(product2, rows2, cols1);
    } else {
        printf("Cannot multiply: inner dimensions don't match (%d != %d)\n", cols2, rows1);
    }
    
    return 0;
}