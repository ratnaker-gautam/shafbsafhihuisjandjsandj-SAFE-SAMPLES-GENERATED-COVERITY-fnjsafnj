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

int sum_diagonal(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int sum = 0;
    int i = 0;
    while (i < size) {
        sum += matrix[i][i];
        i++;
    }
    return sum;
}

int sum_all_elements(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int total = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            total += matrix[i][j];
        }
    }
    return total;
}

int main() {
    int size;
    char input[20];
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input: not a number\n");
        return 1;
    }
    
    if (val < 1 || val > MAX_SIZE) {
        fprintf(stderr, "Invalid size: must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    size = (int)val;
    if (size != val) {
        fprintf(stderr, "Size out of integer range\n");
        return 1;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    fill_matrix(matrix, size);
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, size);
    
    printf("\nSum of diagonal: %d\n", sum_diagonal(matrix, size));
    printf("Sum of all elements: %d\n", sum_all_elements(matrix, size));
    
    transpose_matrix(matrix, size);
    
    printf("\nTransposed matrix:\n");
    print_matrix(matrix, size);
    
    printf("\nSum of diagonal after transpose: %d\n", sum_diagonal(matrix, size));
    printf("Sum of all elements after transpose: %d\n", sum_all_elements(matrix, size));
    
    return 0;
}