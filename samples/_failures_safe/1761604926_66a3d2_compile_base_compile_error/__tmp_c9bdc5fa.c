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
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                 int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int k = 0;
    while (k < rows) {
        int l = 0;
        do {
            if (a[k][l] > 0 && b[k][l] > INT_MAX - a[k][l]) {
                result[k][l] = INT_MAX;
            } else if (a[k][l] < 0 && b[k][l] < INT_MIN - a[k][l]) {
                result[k][l] = INT_MIN;
            } else {
                result[k][l] = a[k][l] + b[k][l];
            }
            l++;
        } while (l < cols);
        k++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE], int rows, int cols, int inner) {
    for (int i = 0; i < rows; i++) {
        int j = 0;
        while (j < cols) {
            int sum = 0;
            for (int k = 0; k < inner; k++) {
                if (a[i][k] != 0 && b[k][j] != 0) {
                    if (a[i][k] > 0) {
                        if (b[k][j] > INT_MAX / a[i][k]) {
                            sum = INT_MAX;
                            break;
                        }
                    } else if (a[i][k] < 0) {
                        if (b[k][j] < INT_MIN / a[i][k]) {
                            sum = INT_MIN;
                            break;
                        }
                    }
                    sum += a[i][k] * b[k][j];
                    if ((sum > 0 && a[i][k] * b[k][j] > 0 && sum < 0) ||
                        (sum < 0 && a[i][k] * b[k][j] < 0 && sum > 0)) {
                        sum = (a[i][k] > 0) ? INT_MAX : INT_MIN;
                        break;
                    }
                }
            }
            result[i][j] = sum;
            j++;
        }
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE],
                     int rows, int cols) {
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            result[j][i] = matrix[i][j];
            j++;
        }
        i++;
    } while (i < rows);
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter dimensions of first matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || rows1 > MAX_SIZE || 
        cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter dimensions of second matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || rows2 > MAX_SIZE || 
        cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        int j = 0;
        while (j < cols2) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            j++;
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        add_matrices(matrix1, matrix2, result, rows1, cols1);
        print_matrix(result, rows1, cols1);
    } else {
        printf("Cannot add: dimension mismatch\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2)