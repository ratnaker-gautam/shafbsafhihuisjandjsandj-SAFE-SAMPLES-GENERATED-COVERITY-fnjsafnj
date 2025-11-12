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
        printf("Invalid dimensions. Must be between 1 and %d\n", MAX_DIM);
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
        printf("Invalid dimensions. Must be between 1 and %d\n", MAX_DIM);
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
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                    printf("%d ", result[i][j]);
                }
                printf("\n");
            }
            break;
            
        case 4:
            printf("Transpose of first matrix:\n");
            for (int i = 0; i < cols1; i++) {
                for (int j = 0; j < rows1; j++) {
                    printf("%d ", matrix1[j][i]);
                }
                printf("\n");
            }
            break;
            
        case 5:
            printf("Transpose of second matrix:\n");
            for (int i = 0; i < cols2; i++) {
                for (int j = 0; j < rows2; j++) {
                    printf("%d ", matrix2[j][i]);
                }
                printf("\n");
            }
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}