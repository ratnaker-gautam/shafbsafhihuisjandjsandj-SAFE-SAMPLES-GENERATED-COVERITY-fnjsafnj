//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    printf("Enter first matrix elements:\n");
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
    
    printf("Enter second matrix elements:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            printf("%d ", matrix1[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }
    
    if (rows1 == rows2 && cols1 == cols2) {
        printf("\nMatrix Addition:\n");
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
    } else {
        printf("\nMatrices cannot be added (dimensions differ)\n");
    }
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                long long sum = 0;
                for (int k = 0; k < cols1; k++) {
                    long long product = (long long)matrix1[i][k] * matrix2[k][j];
                    if (sum > 0 && product > LLONG_MAX - sum) {
                        printf("Integer overflow detected\n");
                        return 1;
                    }
                    if (sum < 0 && product < LLONG_MIN - sum) {
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
    } else {
        printf("\nMatrices cannot be multiplied (incompatible dimensions)\n");
    }
    
    if (rows1 == cols1) {
        printf("\nMatrix 1 Transpose:\n");
        for (int i = 0; i < cols1; i++) {
            for (int j = 0; j < rows1; j++) {
                printf("%d ", matrix1[j][i]);
            }
            printf("\n");
        }
    } else {
        printf("\nMatrix 1 is not square, cannot transpose\n");
    }
    
    return 0;
}