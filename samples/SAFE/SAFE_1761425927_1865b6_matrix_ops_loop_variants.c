//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int counter = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = counter++;
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int i = 0;
    while (i < size) {
        int j = 0;
        do {
            printf("%4d", matrix[i][j]);
            j++;
        } while (j < size);
        printf("\n");
        i++;
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int temp;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
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
        printf("Size must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    fill_matrix(matrix, size);
    
    printf("Original matrix:\n");
    print_matrix(matrix, size);
    
    transpose_matrix(matrix, size);
    
    printf("Transposed matrix:\n");
    print_matrix(matrix, size);
    
    int sum = 0;
    int *ptr = &matrix[0][0];
    int total_elements = size * size;
    for (int i = 0; i < total_elements; i++) {
        sum += *(ptr + i);
    }
    
    printf("Sum of all elements: %d\n", sum);
    
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    int *p = &matrix[0][0];
    int count = 0;
    while (count < total_elements) {
        if (*p < min_val) min_val = *p;
        if (*p > max_val) max_val = *p;
        p++;
        count++;
    }
    
    printf("Minimum value: %d\n", min_val);
    printf("Maximum value: %d\n", max_val);
    
    return 0;
}