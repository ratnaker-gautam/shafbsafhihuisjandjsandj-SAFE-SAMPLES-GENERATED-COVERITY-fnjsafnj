//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void matrix_multiply(int *a, int *b, int *result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            *(result + i * cols_b + j) = 0;
            for (int k = 0; k < cols_a; k++) {
                *(result + i * cols_b + j) += *(a + i * cols_a + k) * *(b + k * cols_b + j);
            }
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Please enter a valid integer between %d and %d\n", min, max);
    }
}

int main() {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix A dimensions:\n");
    rows_a = read_int("Rows (1-10): ", 1, MAX_SIZE);
    cols_a = read_int("Columns (1-10): ", 1, MAX_SIZE);
    
    printf("Matrix B dimensions:\n");
    rows_b = read_int("Rows (1-10): ", 1, MAX_SIZE);
    cols_b = read_int("Columns (1-10): ", 1, MAX_SIZE);
    
    if (cols_a != rows_b) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication\n", cols_a, rows_b);
        return 1;
    }
    
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("\nEnter Matrix A elements:\n");
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            printf("A[%d][%d]: ", i, j);
            matrix_a[i][j] = read_int("", -1000, 1000);
        }
    }
    
    printf("\nEnter Matrix B elements:\n");
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            printf("B[%d][%d]: ", i, j);
            matrix_b[i][j] = read_int("", -1000, 1000);
        }
    }
    
    memset(result, 0, sizeof(result));
    matrix_multiply((int *)matrix_a, (int *)matrix_b, (int *)result, rows_a, cols_a, cols_b);
    
    printf("\nMatrix A:\n");
    print_matrix((int *)matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix((int *)matrix_b, rows_b, cols_b);
    
    printf("\nResult (A × B):\n");
    print_matrix((int *)result, rows_a, cols_b);
    
    return 0;
}