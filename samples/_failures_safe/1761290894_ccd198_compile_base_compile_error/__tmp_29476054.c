//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

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
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            result[j][i] = mat[i][j];
        }
    }
}

void print_matrix(int rows, int cols, int mat[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            printf("%6d", mat[i][j]);
            j++;
        } while (j < cols);
        printf("\n");
        i++;
    }
}

int read_int(const char* prompt, int min, int max) {
    char buffer[100];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

void read_matrix(int rows, int cols, int mat[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            printf("Enter element [%d][%d]: ", i, j);
            mat[i][j] = read_int("", INT_MIN, INT_MAX);
            j++;
        } while (j < cols);
        i++;
    }
}

int main() {
    int mat1[MAX_SIZE][MAX_SIZE];
    int mat2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    int rows1, cols1, rows2, cols2;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    printf("Enter elements for matrix 1:\n");
    read_matrix(rows1, cols1, mat1);
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    printf("Enter elements for matrix 2:\n");
    read_matrix(rows2, cols2, mat2);
    
    while (1) {
        printf("\nMatrix Operations:\n");
        printf("1. Matrix Multiplication\n");
        printf("2. Matrix Transpose (Matrix 1)\n");
        printf("3. Matrix Transpose (Matrix 2)\n");
        printf("4. Exit\n");
        
        choice = read_int("Select operation: ", 1, 4);
        
        switch (choice) {
            case 1:
                if (cols1 != rows2) {
                    printf("Error: Columns of matrix 1 must equal rows of matrix 2 for multiplication.\n");
                    break;
                }
                memset(result, 0, sizeof(result));
                matrix_multiply(rows1, cols1, rows2, cols2, mat1, mat2, result);
                printf("Matrix multiplication result:\n");
                print_matrix(rows1, cols2, result);
                break;
                
            case 2:
                memset(result, 0, sizeof(result));
                matrix_transpose(rows1, cols1, mat1, result);
                printf("Matrix 1 transpose:\n");
                print_matrix(cols1, rows1, result);
                break;
                
            case 3:
                memset(result, 0, sizeof(result));
                matrix_transpose(rows2