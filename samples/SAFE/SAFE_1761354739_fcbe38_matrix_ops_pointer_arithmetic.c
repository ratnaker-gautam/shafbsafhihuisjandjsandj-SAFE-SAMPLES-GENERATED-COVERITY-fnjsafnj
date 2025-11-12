//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void add_matrices(int *a, int *b, int *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + i * cols + j) = *(a + i * cols + j) + *(b + i * cols + j);
        }
    }
}

void multiply_matrices(int *a, int *b, int *result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
}

int read_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%d", matrix + i * cols + j) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    int rows1, cols1, rows2, cols2;
    int operation;
    
    printf("Enter dimensions of first matrix (rows columns): ");
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > MAX_SIZE || cols1 > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions of second matrix (rows columns): ");
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > MAX_SIZE || cols2 > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("Enter choice (1 or 2): ");
    if (scanf("%d", &operation) != 1 || (operation != 1 && operation != 2)) {
        printf("Invalid operation choice\n");
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter elements for first matrix:\n");
    if (!read_matrix((int *)matrix1, rows1, cols1)) {
        printf("Error reading matrix elements\n");
        return 1;
    }
    
    printf("Enter elements for second matrix:\n");
    if (!read_matrix((int *)matrix2, rows2, cols2)) {
        printf("Error reading matrix elements\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix((int *)matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    print_matrix((int *)matrix2, rows2, cols2);
    
    if (operation == 1) {
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Matrix dimensions must match for addition\n");
            return 1;
        }
        add_matrices((int *)matrix1, (int *)matrix2, (int *)result, rows1, cols1);
        printf("\nResult of addition:\n");
        print_matrix((int *)result, rows1, cols1);
    } else {
        if (cols1 != rows2) {
            printf("Matrix dimensions incompatible for multiplication\n");
            return 1;
        }
        multiply_matrices((int *)matrix1, (int *)matrix2, (int *)result, rows1, cols1, cols2);
        printf("\nResult of multiplication:\n");
        print_matrix((int *)result, rows1, cols2);
    }
    
    return 0;
}