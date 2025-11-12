//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

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

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter element: ", INT_MIN / MAX_DIM, INT_MAX / MAX_DIM);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] > INT_MAX - b[i][j]) || (b[i][j] < 0 && a[i][j] < INT_MIN - b[i][j])) {
                result[i][j] = (a[i][j] > 0) ? INT_MAX : INT_MIN;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}

void multiply_matrices(int ra, int ca, int a[MAX_DIM][MAX_DIM], int rb, int cb, int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < ra; i++) {
        for (int j = 0; j < cb; j++) {
            long sum = 0;
            for (int k = 0; k < ca; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    sum = (product > 0) ? INT_MAX : INT_MIN;
                    break;
                }
                sum += product;
            }
            result[i][j] = (sum > INT_MAX) ? INT_MAX : (sum < INT_MIN) ? INT_MIN : (int)sum;
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    printf("Fill matrix 1:\n");
    fill_matrix(rows1, cols1, matrix1);
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    printf("Fill matrix 2:\n");
    fill_matrix(rows2, cols2, matrix2);
    
    printf("Matrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    printf("Matrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Exit\n");
        
        choice = read_int("Select operation: ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                } else {
                    add_matrices(rows1, cols1, matrix1, matrix2, result);
                    printf("Result of addition:\n");
                    print_matrix(rows1, cols1, result);
                }
                break;
            case 2:
                if (cols1 != rows2) {
                    printf("Error: Columns of matrix 1 must equal rows of matrix 2 for multiplication.\n");
                } else {
                    multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
                    printf("Result of multiplication:\n");
                    print_matrix(rows1