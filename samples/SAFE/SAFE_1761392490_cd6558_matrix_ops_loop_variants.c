//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int rows, int cols, int matrix[rows][cols]) {
    int counter = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int src[rows][cols], int dest[cols][rows]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            dest[j][i] = src[i][j];
            j++;
        }
        i++;
    }
}

void multiply_matrices(int r1, int c1, int m1[r1][c1], int r2, int c2, int m2[r2][c2], int result[r1][c2]) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            int k = 0;
            do {
                if (k >= c1) break;
                result[i][j] += m1[i][k] * m2[k][j];
                k++;
            } while (k < c1);
        }
    }
}

int get_valid_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

int main(void) {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = get_valid_int("Enter rows for first matrix (1-8): ", 1, MAX_SIZE);
    int cols1 = get_valid_int("Enter columns for first matrix (1-8): ", 1, MAX_SIZE);
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows1, cols1, matrix1);
    
    printf("\nFirst matrix:\n");
    print_matrix(rows1, cols1, matrix1);
    
    int rows2 = get_valid_int("\nEnter rows for second matrix (1-8): ", 1, MAX_SIZE);
    int cols2 = get_valid_int("Enter columns for second matrix (1-8): ", 1, MAX_SIZE);
    
    int matrix2[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows2, cols2, matrix2);
    
    printf("\nSecond matrix:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nTranspose of first matrix:\n");
    int transpose[MAX_SIZE][MAX_SIZE];
    transpose_matrix(rows1, cols1, matrix1, transpose);
    print_matrix(cols1, rows1, transpose);
    
    if (cols1 == rows2) {
        printf("\nMatrix multiplication result:\n");
        int product[MAX_SIZE][MAX_SIZE];
        multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, product);
        print_matrix(rows1, cols2, product);
    } else {
        printf("\nCannot multiply matrices: columns of first (%d) != rows of second (%d)\n", cols1, rows2);
    }
    
    printf("\nElement-wise operations (using different loop styles):\n");
    printf("First matrix doubled:\n");
    
    int doubled[MAX_SIZE][MAX_SIZE];
    int i = 0;
    while (i < rows1) {
        int j = 0;
        while (j < cols1) {
            doubled[i][j] = matrix1[i][j] * 2;
            j++;
        }
        i++;
    }
    print_matrix(rows1, cols1, doubled);
    
    printf("\nMatrix sum using do-while:\n");
    int sum = 0;
    i = 0;
    do {
        int j = 0;
        do {
            sum += matrix1[i][j];
            j++;
        } while (j < cols1);
        i++;
    } while (i < rows1);
    printf("Sum of first matrix elements: %d\n", sum);
    
    return 0;
}