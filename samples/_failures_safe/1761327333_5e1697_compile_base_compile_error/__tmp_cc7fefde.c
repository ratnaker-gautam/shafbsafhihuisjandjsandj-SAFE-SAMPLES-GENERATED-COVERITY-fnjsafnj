//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void fill_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void add_matrices(int *a, int *b, int *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            if ((*(a + idx) > 0 && *(b + idx) > INT_MAX - *(a + idx)) ||
                (*(a + idx) < 0 && *(b + idx) < INT_MIN - *(a + idx))) {
                printf("Integer overflow detected at [%d][%d]\n", i, j);
                *(result + idx) = 0;
            } else {
                *(result + idx) = *(a + idx) + *(b + idx);
            }
        }
    }
}

void multiply_matrices(int *a, int *b, int *result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            int sum = 0;
            for (int k = 0; k < cols_a; k++) {
                long long product = (long long)*(a + i * cols_a + k) * (long long)*(b + k * cols_b + j);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected at [%d][%d]\n", i, j);
                    sum = 0;
                    break;
                }
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    printf("Integer overflow detected at [%d][%d]\n", i, j);
                    sum = 0;
                    break;
                }
                sum += (int)product;
            }
            *(result + i * cols_b + j) = sum;
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    
    printf("\nFilling matrix 1:\n");
    fill_matrix((int *)matrix1, rows1, cols1);
    
    printf("\nMatrix 1:\n");
    print_matrix((int *)matrix1, rows1, cols1);
    
    rows2 = read_int("\nEnter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    
    printf("\nFilling matrix 2:\n");
    fill_matrix((int *)matrix2, rows2, cols2);
    
    printf("\nMatrix 2:\n");
    print_matrix((int *)matrix2, rows2, cols2);
    
    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        add_matrices((int *)matrix1, (int *)matrix2, (int *)result, rows1, cols1);
        print_matrix((int *)result, rows1, cols1);
    } else {
        printf("Cannot add matrices: dimensions don't match.\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        multiply_matrices((int *)matrix1, (int *)matrix2, (int *)result, rows1, cols1, cols2);
        print_matrix((int *)result, rows1, cols2);