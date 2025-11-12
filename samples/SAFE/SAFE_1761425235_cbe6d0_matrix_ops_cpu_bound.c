//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

void matrix_multiply(int rows1, int cols1, int rows2, int cols2, 
                    int mat1[MAX_SIZE][MAX_SIZE], 
                    int mat2[MAX_SIZE][MAX_SIZE],
                    int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

void matrix_transpose(int rows, int cols, 
                     int mat[MAX_SIZE][MAX_SIZE],
                     int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = mat[i][j];
        }
    }
}

void print_matrix(int rows, int cols, int mat[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", mat[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int mat1[MAX_SIZE][MAX_SIZE];
    int mat2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    int rows1, cols1, rows2, cols2;
    
    srand(time(NULL));
    
    printf("Enter dimensions for first matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows1 <= 0 || rows1 > MAX_SIZE || cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Generating random matrices...\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            mat1[i][j] = rand() % 100;
        }
    }
    
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            mat2[i][j] = rand() % 100;
        }
    }
    
    printf("\nMatrix 1 (%dx%d):\n", rows1, cols1);
    print_matrix(rows1, cols1, mat1);
    
    printf("\nMatrix 2 (%dx%d):\n", rows2, cols2);
    print_matrix(rows2, cols2, mat2);
    
    if (cols1 == rows2) {
        printf("\nMatrix multiplication result (%dx%d):\n", rows1, cols2);
        matrix_multiply(rows1, cols1, rows2, cols2, mat1, mat2, result);
        print_matrix(rows1, cols2, result);
    } else {
        printf("\nCannot multiply matrices: incompatible dimensions\n");
    }
    
    printf("\nTranspose of Matrix 1 (%dx%d):\n", cols1, rows1);
    matrix_transpose(rows1, cols1, mat1, result);
    print_matrix(cols1, rows1, result);
    
    printf("\nTranspose of Matrix 2 (%dx%d):\n", cols2, rows2);
    matrix_transpose(rows2, cols2, mat2, result);
    print_matrix(cols2, rows2, result);
    
    return 0;
}