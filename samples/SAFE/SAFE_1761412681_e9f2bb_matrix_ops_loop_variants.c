//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void matrix_multiply(int rows1, int cols1, int rows2, int cols2, 
                    int mat1[MAX_SIZE][MAX_SIZE], 
                    int mat2[MAX_SIZE][MAX_SIZE],
                    int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows1) {
        int j = 0;
        do {
            result[i][j] = 0;
            int k = 0;
            for (; k < cols1; k++) {
                if (mat1[i][k] > 0 && mat2[k][j] > 0) {
                    if (mat1[i][k] > INT_MAX / mat2[k][j]) {
                        result[i][j] = 0;
                        break;
                    }
                } else if (mat1[i][k] < 0 && mat2[k][j] < 0) {
                    if (mat1[i][k] < INT_MAX / mat2[k][j]) {
                        result[i][j] = 0;
                        break;
                    }
                }
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
            j++;
        } while (j < cols2);
        i++;
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    for (; i < rows; i++) {
        int j = 0;
        while (j < cols) {
            printf("%6d", matrix[i][j]);
            j++;
        }
        printf("\n");
    }
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[32];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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
        break;
    }
    
    return value;
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), 
                    "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
            j++;
        }
        i++;
    } while (i < rows);
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
    
    rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_SIZE);
    rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Error: Columns of matrix 1 must equal rows of matrix 2.\n");
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE] = {0};
    int matrix2[MAX_SIZE][MAX_SIZE] = {0};
    int result[MAX_SIZE][MAX_SIZE] = {0};
    
    printf("\nEnter elements for matrix 1:\n");
    read_matrix(rows1, cols1, matrix1);
    
    printf("\nEnter elements for matrix 2:\n");
    read_matrix(rows2, cols2, matrix2);
    
    matrix_multiply(rows1, cols1, rows2, cols2, matrix1, matrix2, result);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nResult of multiplication:\n");
    print_matrix(rows1, cols2, result);
    
    return 0;
}