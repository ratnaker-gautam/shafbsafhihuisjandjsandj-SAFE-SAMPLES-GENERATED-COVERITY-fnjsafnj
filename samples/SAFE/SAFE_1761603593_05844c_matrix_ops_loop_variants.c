//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            dst[j][i] = src[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void add_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        int j = 0;
        while (j < cols) {
            if ((b[i][j] > 0 && a[i][j] > INT_MAX - b[i][j]) || 
                (b[i][j] < 0 && a[i][j] < INT_MIN - b[i][j])) {
                printf("Integer overflow detected at position [%d][%d]\n", i, j);
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
            j++;
        }
    }
}

int main() {
    int rows, cols;
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1 || rows < 1 || rows > MAX_SIZE) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1 || cols < 1 || cols > MAX_SIZE) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    printf("Enter elements for matrix A (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix_a[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements for matrix B (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        int j = 0;
        while (j < cols) {
            if (scanf("%d", &matrix_b[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
            j++;
        }
    }
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows, cols);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows, cols);
    
    printf("\nMatrix A + B:\n");
    add_matrices(matrix_a, matrix_b, result, rows, cols);
    print_matrix(result, rows, cols);
    
    if (rows == cols) {
        int transposed[MAX_SIZE][MAX_SIZE];
        printf("\nTranspose of A:\n");
        transpose_matrix(matrix_a, transposed, rows, cols);
        print_matrix(transposed, cols, rows);
    } else {
        printf("\nTranspose not shown (matrix not square)\n");
    }
    
    int sum = 0;
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            sum += matrix_a[i][j];
            j++;
        }
        i++;
    } while (i < rows);
    printf("\nSum of all elements in matrix A: %d\n", sum);
    
    return 0;
}