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
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE], const char* name) {
    printf("Enter values for matrix %s (%d x %d):\n", name, rows, cols);
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
            j++;
        }
        i++;
    } while (i < rows);
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int mat1[MAX_SIZE][MAX_SIZE];
    int mat2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum matrix size: %d x %d\n\n", MAX_SIZE, MAX_SIZE);
    
    printf("First matrix dimensions:\n");
    rows1 = read_int("  Rows: ", 1, MAX_SIZE);
    cols1 = read_int("  Columns: ", 1, MAX_SIZE);
    
    printf("Second matrix dimensions:\n");
    rows2 = read_int("  Rows: ", 1, MAX_SIZE);
    cols2 = read_int("  Columns: ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Error: Cannot multiply matrices. Columns of first (%d) must equal rows of second (%d).\n", cols1, rows2);
        return 1;
    }
    
    read_matrix(rows1, cols1, mat1, "A");
    read_matrix(rows2, cols2, mat2, "B");
    
    memset(result, 0, sizeof(result));
    matrix_multiply(rows1, cols1, rows2, cols2, mat1, mat2, result);
    
    printf("\nMatrix A:\n");
    print_matrix(rows1, cols1, mat1);
    
    printf("\nMatrix B:\n");
    print_matrix(rows2, cols2, mat2);
    
    printf("\nResult (A x B):\n");
    print_matrix(rows1, cols2, result);
    
    return 0;
}