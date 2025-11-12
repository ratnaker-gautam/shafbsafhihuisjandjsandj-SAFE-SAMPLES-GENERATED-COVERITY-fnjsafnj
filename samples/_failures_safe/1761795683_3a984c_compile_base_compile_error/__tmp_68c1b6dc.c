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
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN / 100, INT_MAX / 100);
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
                long product = (long)a[i][k] * b[k][j];
                if ((a[i][k] != 0 && b[k][j] != 0 && (product / a[i][k] != b[k][j] || product > INT_MAX || product < INT_MIN)) || 
                    (sum > 0 && product > INT_MAX - sum) || (sum < 0 && product < INT_MIN - sum)) {
                    sum = (sum > 0) ? INT_MAX : INT_MIN;
                    break;
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    sum = (sum > 0) ? INT_MAX : INT_MIN;
                    break;
                }
            }
            result[i][j] = (int)sum;
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

int main() {
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
            
            printf("\nFirst matrix:\n");
            print_matrix(rows1, cols1, matrix1);
            printf("\nSecond matrix:\n");
            print_matrix(rows2, cols2,