//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void transpose_matrix(int *src, int *dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int main() {
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
    
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input for matrix element\n");
                return 1;
            }
        }
    }
    
    printf("\nOriginal matrix:\n");
    print_matrix((int *)matrix, rows, cols);
    
    if (rows <= MAX_SIZE && cols <= MAX_SIZE) {
        transpose_matrix((int *)matrix, (int *)transposed, rows, cols);
        printf("\nTransposed matrix:\n");
        print_matrix((int *)transposed, cols, rows);
    }
    
    int trace = 0;
    if (rows == cols) {
        for (int i = 0; i < rows; i++) {
            trace += *(*(matrix + i) + i);
        }
        printf("\nMatrix trace: %d\n", trace);
    } else {
        printf("\nMatrix is not square, no trace calculated\n");
    }
    
    return 0;
}