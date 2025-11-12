//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void matrix_multiply(int rows_a, int cols_a, int rows_b, int cols_b, 
                    int matrix_a[MAX_SIZE][MAX_SIZE], 
                    int matrix_b[MAX_SIZE][MAX_SIZE],
                    int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows_a) {
        int j = 0;
        do {
            result[i][j] = 0;
            int k = 0;
            for (; k < cols_a; k++) {
                if (matrix_a[i][k] > 0 && matrix_b[k][j] > 0) {
                    if (matrix_a[i][k] > INT_MAX / matrix_b[k][j]) {
                        result[i][j] = 0;
                        break;
                    }
                } else if (matrix_a[i][k] < 0 && matrix_b[k][j] < 0) {
                    if (matrix_a[i][k] < INT_MAX / matrix_b[k][j]) {
                        result[i][j] = 0;
                        break;
                    }
                }
                result[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
            j++;
        } while (j < cols_b);
        i++;
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        for (; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
        i++;
    }
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    int valid = 0;
    
    do {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            continue;
        }
        
        if (temp >= min && temp <= max) {
            value = (int)temp;
            valid = 1;
        }
    } while (!valid);
    
    return value;
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        for (; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
        i++;
    }
}

int main(void) {
    int rows_a, cols_a, rows_b, cols_b;
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix A dimensions (max %dx%d):\n", MAX_SIZE, MAX_SIZE);
    rows_a = read_int("Rows: ", 1, MAX_SIZE);
    cols_a = read_int("Columns: ", 1, MAX_SIZE);
    
    printf("Matrix B dimensions (max %dx%d):\n", MAX_SIZE, MAX_SIZE);
    rows_b = read_int("Rows: ", 1, MAX_SIZE);
    cols_b = read_int("Columns: ", 1, MAX_SIZE);
    
    if (cols_a != rows_b) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    
    printf("Enter elements for Matrix A:\n");
    read_matrix(rows_a, cols_a, matrix_a);
    
    printf("Enter elements for Matrix B:\n");
    read_matrix(rows_b, cols_b, matrix_b);
    
    printf("\nMatrix A:\n");
    print_matrix(rows_a, cols_a, matrix_a);
    
    printf("\nMatrix B:\n");
    print_matrix(rows_b, cols_b, matrix_b);
    
    matrix_multiply(rows_a, cols_a, rows_b, cols_b, matrix_a, matrix_b, result);
    
    printf("\nResult (A × B):\n");
    print_matrix(rows_a, cols_b, result);
    
    return 0;
}