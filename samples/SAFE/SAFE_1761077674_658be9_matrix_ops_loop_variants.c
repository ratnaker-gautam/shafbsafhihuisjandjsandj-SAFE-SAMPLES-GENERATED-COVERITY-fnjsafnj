//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

void matrix_multiply(int rows_a, int cols_a, int rows_b, int cols_b, 
                    int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], 
                    int result[MAX_DIM][MAX_DIM]) {
    int i = 0;
    while (i < rows_a) {
        int j = 0;
        do {
            result[i][j] = 0;
            int k = 0;
            for (; k < cols_a; k++) {
                if (a[i][k] > 0 && b[k][j] > 0) {
                    if (a[i][k] > INT_MAX / b[k][j]) {
                        result[i][j] = INT_MAX;
                        break;
                    }
                } else if (a[i][k] < 0 && b[k][j] < 0) {
                    if (a[i][k] < INT_MAX / b[k][j]) {
                        result[i][j] = INT_MAX;
                        break;
                    }
                }
                result[i][j] += a[i][k] * b[k][j];
            }
            j++;
        } while (j < cols_b);
        i++;
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
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
    char buffer[100];
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
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        valid = 1;
    } while (!valid);
    
    return value;
}

void read_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
            j++;
        } while (j < cols);
        i++;
    }
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    int matrix_a[MAX_DIM][MAX_DIM];
    int matrix_b[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix A dimensions:\n");
    rows_a = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols_a = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Matrix B dimensions:\n");
    rows_b = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols_b = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    if (cols_a != rows_b) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", cols_a, rows_b);
        return 1;
    }
    
    printf("\nEnter Matrix A elements:\n");
    read_matrix(rows_a, cols_a, matrix_a);
    
    printf("\nEnter Matrix B elements:\n");
    read_matrix(rows_b, cols_b, matrix_b);
    
    memset(result, 0, sizeof(result));
    matrix_multiply(rows_a, cols_a, rows_b, cols_b, matrix_a, matrix_b, result);
    
    printf("\nMatrix A:\n");
    print_matrix(rows_a, cols_a, matrix_a);
    
    printf("\nMatrix B:\n");
    print_matrix(rows_b, cols_b, matrix_b);
    
    printf("\nResult (A * B):\n");
    print_matrix(rows_a, cols_b, result);
    
    return 0;
}