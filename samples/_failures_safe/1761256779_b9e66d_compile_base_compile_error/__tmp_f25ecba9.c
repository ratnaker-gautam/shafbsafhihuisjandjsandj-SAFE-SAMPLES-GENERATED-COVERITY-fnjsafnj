//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
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

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    printf("Enter %d elements for %d x %d matrix:\n", rows * cols, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void add_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows1, int cols1, int cols2) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < cols1) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected\n");
                    exit(1);
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected\n");
                    exit(1);
                }
                result[i][j] = (int)sum;
                k++;
            }
        }
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            result[j][i] = matrix[i][j];
            j++;
        }
        i++;
    } while (i < rows);
}

int main() {
    int rows1, cols1, rows2, cols2;
    int choice;
    
    printf("Enter dimensions for first matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > MAX_SIZE || cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);
    printf("Matrix 2:\n");
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nChoose operation:\n");
    printf("1. Add matrices\n");
    printf("2. Multiply matrices\n");
    printf("3. Transpose matrix 1\n");
    printf("4. Transpose matrix 2\n");
    
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrices must have same dimensions for addition\n");
                return 1;
            }
            add_matrices(matrix1, matrix2, result, rows1, cols1);
            printf("\nSum:\n");
            print_matrix(result, rows1, cols1);
            break;
            
        case 2:
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            multiply_matrices(matrix1, matrix2, result, rows1, cols1,