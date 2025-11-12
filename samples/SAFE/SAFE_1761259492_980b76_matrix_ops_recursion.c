//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_SIZE 10

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int recursive_determinant(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    if (size == 1) {
        return matrix[0][0];
    }
    
    int det = 0;
    int sign = 1;
    int submatrix[MAX_SIZE][MAX_SIZE];
    
    for (int col = 0; col < size; col++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                submatrix[sub_i][sub_j] = matrix[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int minor = recursive_determinant(submatrix, size - 1);
        int term = matrix[0][col] * minor;
        
        if (sign == 1) {
            if (det > 0 && term > INT_MAX - det) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            if (det < 0 && term < INT_MIN - det) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            det += term;
        } else {
            if (term == INT_MIN && det == INT_MIN) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            int neg_term = -term;
            if (det > 0 && neg_term > INT_MAX - det) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            if (det < 0 && neg_term < INT_MIN - det) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            det -= term;
        }
        sign = -sign;
    }
    
    return det;
}

int main() {
    int size;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size < 1 || size > MAX_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    
    printf("Enter matrix elements:\n");
    read_matrix(matrix, size);
    
    printf("\nMatrix:\n");
    print_matrix(matrix, size);
    
    int det = recursive_determinant(matrix, size);
    printf("\nDeterminant: %d\n", det);
    
    return 0;
}