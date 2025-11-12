//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[32];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            continue;
        }
        
        if (temp >= min && temp <= max && temp <= INT_MAX && temp >= INT_MIN) {
            value = (int)temp;
            break;
        }
    }
    return value;
}

void read_matrix(Matrix* mat) {
    mat->rows = read_int("Enter rows (1-8): ", 1, MAX_SIZE);
    mat->cols = read_int("Enter cols (1-8): ", 1, MAX_SIZE);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_sum_recursive(const Matrix* mat, int row, int col) {
    if (row >= mat->rows) {
        return 0;
    }
    
    if (col >= mat->cols) {
        return matrix_sum_recursive(mat, row + 1, 0);
    }
    
    int current = mat->data[row][col];
    
    if ((current > 0 && current > INT_MAX - matrix_sum_recursive(mat, row, col + 1)) ||
        (current < 0 && current < INT_MIN - matrix_sum_recursive(mat, row, col + 1))) {
        return 0;
    }
    
    return current + matrix_sum_recursive(mat, row, col + 1);
}

Matrix matrix_add_recursive(const Matrix* a, const Matrix* b, int row, int col) {
    Matrix result;
    result.rows = a->rows;
    result.cols = a->cols;
    
    if (row >= a->rows) {
        return result;
    }
    
    if (col >= a->cols) {
        return matrix_add_recursive(a, b, row + 1, 0);
    }
    
    long sum = (long)a->data[row][col] + (long)b->data[row][col];
    if (sum > INT_MAX || sum < INT_MIN) {
        result.data[row][col] = 0;
    } else {
        result.data[row][col] = (int)sum;
    }
    
    Matrix temp = matrix_add_recursive(a, b, row, col + 1);
    result.data[row][col] = temp.data[row][col];
    
    return result;
}

int main() {
    Matrix mat1, mat2;
    
    printf("Matrix Operations using Recursion\n");
    printf("First matrix:\n");
    read_matrix(&mat1);
    
    printf("Second matrix:\n");
    read_matrix(&mat2);
    
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        printf("Matrices must have same dimensions for addition.\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    int sum1 = matrix_sum_recursive(&mat1, 0, 0);
    int sum2 = matrix_sum_recursive(&mat2, 0, 0);
    
    printf("\nSum of first matrix elements: %d\n", sum1);
    printf("Sum of second matrix elements: %d\n", sum2);
    
    Matrix result = matrix_add_recursive(&mat1, &mat2, 0, 0);
    printf("\nMatrix addition result:\n");
    print_matrix(&result);
    
    return 0;
}