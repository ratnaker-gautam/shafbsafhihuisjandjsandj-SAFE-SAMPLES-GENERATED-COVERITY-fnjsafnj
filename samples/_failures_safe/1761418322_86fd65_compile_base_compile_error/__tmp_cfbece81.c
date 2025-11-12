//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Enter dimensions of first matrix (rows columns): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows1 <= 0 || rows1 > MAX_DIM || cols1 <= 0 || cols1 > MAX_DIM) {
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
    
    printf("Enter dimensions of second matrix (rows columns): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows2 <= 0 || rows2 > MAX_DIM || cols2 <= 0 || cols2 > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix operations menu:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Transpose first matrix\n");
    printf("5. Transpose second matrix\n");
    printf("Enter choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition\n");
                return 1;
            }
            printf("Result of addition:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                        (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                    printf("%d ", result[i][j]);
                }
                printf("\n");
            }
            break;
            
        case 2:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for subtraction\n");
                return 1;
            }
            printf("Result of subtraction:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                    printf("%d ", result[i][j]);
                }
                printf("\n");
            }
            break;
            
        case 3:
            if (cols1 != rows2) {
                printf("Invalid dimensions for multiplication\n");
                return 1;
            }
            printf("Result of multiplication:\n");
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    long long sum = 0;
                    for (int k = 0; k < cols1; k++) {
                        long long product = (long long)matrix1[i][k] * matrix2[k][j];
                        if (product > 0 && sum > LLONG_MAX - product) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                        if (product < 0 && sum < LLONG_MIN - product) {
                            printf("Integer overflow detected\n");
                            return 1;
                        }
                        sum += product;
                    }
                    if (sum > INT_MAX || sum < INT_MIN) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    result[i][j] = (int)sum;
                    printf("%d ", result[i][j]);
                }
                printf("\n");
            }
            break;
            
        case