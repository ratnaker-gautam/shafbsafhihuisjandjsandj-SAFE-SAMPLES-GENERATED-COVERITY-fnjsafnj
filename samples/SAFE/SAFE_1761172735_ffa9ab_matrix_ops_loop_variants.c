//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

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
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

int main(void) {
    int size;
    char input[32];
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (val < 1 || val > MAX_SIZE) {
        fprintf(stderr, "Size out of range\n");
        return 1;
    }
    
    size = (int)val;
    if (size != val) {
        fprintf(stderr, "Size overflow\n");
        return 1;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
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
        if (sum > INT_MAX - *ptr) {
            fprintf(stderr, "Sum overflow\n");
            return 1;
        }
        sum += *ptr++;
    }
    
    printf("Sum of all elements: %d\n", sum);
    
    int min_val = matrix[0][0];
    int max_val = matrix[0][0];
    int row = 0;
    while (row < size) {
        int col = 0;
        do {
            if (matrix[row][col] < min_val) {
                min_val = matrix[row][col];
            }
            if (matrix[row][col] > max_val) {
                max_val = matrix[row][col];
            }
            col++;
        } while (col < size);
        row++;
    }
    
    printf("Minimum value: %d\n", min_val);
    printf("Maximum value: %d\n", max_val);
    
    return 0;
}