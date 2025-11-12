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

void matrix_transpose(int rows, int cols, 
                     int mat[MAX_SIZE][MAX_SIZE],
                     int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    for (; i < rows; i++) {
        int j = 0;
        while (j < cols) {
            result[j][i] = mat[i][j];
            j++;
        }
    }
}

void print_matrix(int rows, int cols, int mat[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            printf("%6d", mat[i][j]);
            j++;
        }
        printf("\n");
        i++;
    } while (i < rows);
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        char* endptr;
        long val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

void read_matrix(int rows, int cols, int mat[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        for (; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), 
                    "Enter element [%d][%d]: ", i, j);
            mat[i][j] = read_int(prompt, -1000, 1000);
        }
        i++;
    }
}

int main() {
    int mat1[MAX_SIZE][MAX_SIZE] = {0};
    int mat2[MAX_SIZE][MAX_SIZE] = {0};
    int result[MAX_SIZE][MAX_SIZE] = {0};
    
    printf("Matrix Operations Program\n");
    printf("Matrix size limit: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    int rows1 = read_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    int cols1 = read_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    
    printf("Enter elements for matrix 1:\n");
    read_matrix(rows1, cols1, mat1);
    
    int rows2 = read_int("Enter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    int cols2 = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    
    printf("Enter elements for matrix 2:\n");
    read_matrix(rows2, cols2, mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, mat2);
    
    if (cols1 == rows2) {
        printf("\nMatrix multiplication result:\n");
        matrix_multiply(rows1, cols1, rows2, cols2, mat1, mat2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nCannot multiply: columns of matrix 1 != rows of matrix 2\n");
    }
    
    printf("\nTranspose of matrix 1:\n");
    int trans1[MAX_SIZE][MAX_SIZE] = {0};
    matrix_transpose(rows1, cols1, mat1, trans1);
    print_matrix(cols1, rows1, trans1);
    
    printf("\nTranspose of matrix 2:\n");
    int trans2[MAX_SIZE][MAX_SIZE] = {0};
    matrix_transpose(