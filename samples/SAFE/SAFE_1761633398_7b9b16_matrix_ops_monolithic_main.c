//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

int main(void) {
    int rows, cols;
    printf("Enter matrix dimensions (rows columns): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Dimensions must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter elements for first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("Enter elements for second matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix2[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    printf("\nMatrix addition:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                fprintf(stderr, "Error: Integer overflow in addition\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] + matrix2[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix subtraction:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                fprintf(stderr, "Error: Integer overflow in subtraction\n");
                return 1;
            }
            result[i][j] = matrix1[i][j] - matrix2[i][j];
            printf("%6d", result[i][j]);
        }
        printf("\n");
    }
    
    if (rows == cols) {
        printf("\nMatrix multiplication:\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                long long sum = 0;
                for (int k = 0; k < cols; k++) {
                    long long product = (long long)matrix1[i][k] * matrix2[k][j];
                    if (product > INT_MAX || product < INT_MIN) {
                        fprintf(stderr, "Error: Integer overflow in multiplication\n");
                        return 1;
                    }
                    if (sum > 0 && product > LLONG_MAX - sum) {
                        fprintf(stderr, "Error: Integer overflow in sum\n");
                        return 1;
                    }
                    if (sum < 0 && product < LLONG_MIN - sum) {
                        fprintf(stderr, "Error: Integer overflow in sum\n");
                        return 1;
                    }
                    sum += product;
                }
                if (sum > INT_MAX || sum < INT_MIN) {
                    fprintf(stderr, "Error: Integer overflow in final result\n");
                    return 1;
                }
                result[i][j] = (int)sum;
                printf("%6d", result[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("\nMatrix multiplication not possible: matrices are not square\n");
    }
    
    printf("\nMatrix transpose:\n");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            printf("%6d", matrix1[j][i]);
        }
        printf("\n");
    }
    
    return 0;
}