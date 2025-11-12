//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            dst[j][i] = src[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

int main(void) {
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1 || rows < 1 || rows > MAX_SIZE) {
        printf("Invalid input for rows\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1 || cols < 1 || cols > MAX_SIZE) {
        printf("Invalid input for columns\n");
        return 1;
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, rows, cols);
    
    transpose_matrix(matrix, transposed, rows, cols);
    
    printf("\nTransposed matrix:\n");
    print_matrix(transposed, cols, rows);
    
    int sum = 0;
    int k = 0;
    while (k < rows) {
        int m = 0;
        while (m < cols) {
            sum += matrix[k][m];
            m++;
        }
        k++;
    }
    printf("\nSum of all elements: %d\n", sum);
    
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    for (int i = 0; i < rows; i++) {
        int j = 0;
        do {
            if (matrix[i][j] < min_val) {
                min_val = matrix[i][j];
            }
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
            }
            j++;
        } while (j < cols);
    }
    printf("Minimum value: %d\n", min_val);
    printf("Maximum value: %d\n", max_val);
    
    return 0;
}