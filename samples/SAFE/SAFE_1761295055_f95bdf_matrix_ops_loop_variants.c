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
        int j = 0;
        while (j < size) {
            matrix[i][j] = counter++;
            j++;
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int i = 0;
    do {
        int j = 0;
        while (j < size) {
            printf("%4d", matrix[i][j]);
            j++;
        }
        printf("\n");
        i++;
    } while (i < size);
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

int validate_size(int size) {
    if (size <= 0 || size > MAX_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    int size;
    char input[32];
    
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
    
    if (val < INT_MIN || val > INT_MAX) {
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    size = (int)val;
    if (!validate_size(size)) {
        fprintf(stderr, "Invalid size: must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("\nOriginal matrix:\n");
    fill_matrix(matrix, size);
    print_matrix(matrix, size);
    
    printf("\nTransposed matrix:\n");
    transpose_matrix(matrix, size);
    print_matrix(matrix, size);
    
    int sum = 0;
    int count = 0;
    for (int i = 0; i < size; i++) {
        int j = 0;
        while (j < size) {
            if (matrix[i][j] > 0) {
                if (sum > INT_MAX - matrix[i][j]) {
                    fprintf(stderr, "Sum overflow detected\n");
                    return 1;
                }
                sum += matrix[i][j];
                count++;
            }
            j++;
        }
    }
    
    if (count > 0) {
        printf("\nAverage of positive elements: %.2f\n", (double)sum / count);
    } else {
        printf("\nNo positive elements to average\n");
    }
    
    return 0;
}