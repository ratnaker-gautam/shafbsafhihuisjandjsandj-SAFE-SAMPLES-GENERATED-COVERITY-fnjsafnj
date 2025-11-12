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
            matrix[i][j] = read_int("Enter element: ", -1000, 1000);
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
                result[i][j] = 0;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}

void multiply_matrices(int a_rows, int a_cols, int b_cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            long sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > 0 && sum > LONG_MAX - product) {
                    sum = 0;
                    break;
                }
                if (product < 0 && sum < LONG_MIN - product) {
                    sum = 0;
                    break;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                result[i][j] = 0;
            } else {
                result[i][j] = (int)sum;
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main(void) {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    
    while (1) {
        printf("\n1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose matrix\n");
        printf("4. Exit\n");
        
        choice = read_int("Select operation: ", 1, 4);
        
        if (choice == 4) {
            break;
        }
        
        if (choice == 1 || choice == 2) {
            printf("\nFirst matrix:\n");
            rows1 = read_int("Enter rows (1-10): ", 1, MAX_DIM);
            cols1 = read_int("Enter columns (1-10): ", 1, MAX_DIM);
            fill_matrix(rows1, cols1, matrix1);
            
            printf("\nSecond matrix:\n");
            rows2 = read_int("Enter rows (1-10): ", 1, MAX_DIM);
            cols2 = read_int("Enter columns (1-10): ", 1, MAX_DIM);
            fill_matrix(rows2, cols2, matrix2);
            
            if (choice == 1) {
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                    continue;
                }
                add_matrices(rows1, cols1, matrix1, matrix2, result);
                printf("\nMatrix A:\n");
                print_matrix(rows1, cols1, matrix1);
                printf("\nMatrix B:\n");
                print_matrix(rows2, cols2, matrix2);
                printf("\nA + B:\n");
                print_matrix(rows1, cols