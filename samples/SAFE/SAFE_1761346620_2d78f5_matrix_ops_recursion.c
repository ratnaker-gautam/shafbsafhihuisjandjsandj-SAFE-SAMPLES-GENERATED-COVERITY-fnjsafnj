//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void recursive_transpose(int rows, int cols, int matrix[rows][cols], int result[cols][rows], int i, int j) {
    if (i >= rows) return;
    if (j >= cols) {
        recursive_transpose(rows, cols, matrix, result, i + 1, 0);
        return;
    }
    result[j][i] = matrix[i][j];
    recursive_transpose(rows, cols, matrix, result, i, j + 1);
}

void recursive_print_matrix(int rows, int cols, int matrix[rows][cols], int i, int j) {
    if (i >= rows) return;
    if (j >= cols) {
        printf("\n");
        recursive_print_matrix(rows, cols, matrix, i + 1, 0);
        return;
    }
    printf("%d ", matrix[i][j]);
    recursive_print_matrix(rows, cols, matrix, i, j + 1);
}

int read_positive_int(const char* prompt, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value <= 0 || value > max_val) {
            printf("Value must be between 1 and %d\n", max_val);
            continue;
        }
        
        return value;
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    char buffer[32];
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            while (1) {
                printf("Enter element [%d][%d]: ", i, j);
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("Input error\n");
                    exit(1);
                }
                
                if (sscanf(buffer, "%d", &matrix[i][j]) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    continue;
                }
                
                break;
            }
        }
    }
}

int main() {
    int rows, cols;
    
    printf("Matrix Transpose using Recursion\n");
    printf("===============================\n");
    
    rows = read_positive_int("Enter number of rows (1-10): ", MAX_SIZE);
    cols = read_positive_int("Enter number of columns (1-10): ", MAX_SIZE);
    
    int matrix[rows][cols];
    int transpose[cols][rows];
    
    printf("\nEnter matrix elements:\n");
    read_matrix(rows, cols, matrix);
    
    printf("\nOriginal matrix:\n");
    recursive_print_matrix(rows, cols, matrix, 0, 0);
    
    recursive_transpose(rows, cols, matrix, transpose, 0, 0);
    
    printf("\nTransposed matrix:\n");
    recursive_print_matrix(cols, rows, transpose, 0, 0);
    
    return 0;
}