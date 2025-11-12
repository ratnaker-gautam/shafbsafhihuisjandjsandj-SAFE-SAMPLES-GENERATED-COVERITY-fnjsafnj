//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(matrix, i, j, cols) (matrix)[(i) * (cols) + (j)]
#define SWAP(a, b) do { int temp = (a); (a) = (b); (b) = temp; } while(0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_ELEM(matrix, i, j, cols) = rand() % 100;
        }
    }
}

void print_matrix(const int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void transpose_matrix(const int* src, int* dst, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_ELEM(dst, j, i, rows) = MATRIX_ELEM(src, i, j, cols);
        }
    }
}

int multiply_matrices(const int* a, int a_rows, int a_cols, const int* b, int b_rows, int b_cols, int* result) {
    if (a_cols != b_rows) {
        return 0;
    }
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += MATRIX_ELEM(a, i, k, a_cols) * MATRIX_ELEM(b, k, j, b_cols);
            }
            MATRIX_ELEM(result, i, j, b_cols) = sum;
        }
    }
    return 1;
}

int main() {
    srand(42);
    
    int rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    
    int rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    
    fill_matrix(matrix1, rows1, cols1);
    fill_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    int transposed[MAX_DIM * MAX_DIM];
    printf("\nTranspose of Matrix 1:\n");
    transpose_matrix(matrix1, transposed, rows1, cols1);
    print_matrix(transposed, cols1, rows1);
    
    if (cols1 == rows2) {
        int product[MAX_DIM * MAX_DIM];
        if (multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, product)) {
            printf("\nMatrix 1 * Matrix 2:\n");
            print_matrix(product, rows1, cols2);
        }
    } else {
        printf("\nCannot multiply matrices: columns of matrix 1 (%d) != rows of matrix 2 (%d)\n", cols1, rows2);
    }
    
    return 0;
}