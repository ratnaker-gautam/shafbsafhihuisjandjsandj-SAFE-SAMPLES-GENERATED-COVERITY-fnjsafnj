//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int value = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = value++;
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

int get_valid_size(void) {
    char input[32];
    int size;
    
    while (1) {
        printf("Enter matrix size (1-%d): ", MAX_SIZE);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        
        char *endptr;
        long val = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (val < 1 || val > MAX_SIZE) {
            printf("Size must be between 1 and %d\n", MAX_SIZE);
            continue;
        }
        
        size = (int)val;
        break;
    }
    
    return size;
}

int main(void) {
    int matrix[MAX_SIZE][MAX_SIZE];
    int size;
    
    size = get_valid_size();
    
    printf("\nOriginal matrix:\n");
    fill_matrix(matrix, size);
    print_matrix(matrix, size);
    
    printf("\nTransposed matrix:\n");
    transpose_matrix(matrix, size);
    print_matrix(matrix, size);
    
    int trace = 0;
    for (int i = 0; i < size; i++) {
        trace += matrix[i][i];
    }
    printf("\nMatrix trace: %d\n", trace);
    
    int row_sums[MAX_SIZE];
    memset(row_sums, 0, sizeof(row_sums));
    
    int i = 0;
    while (i < size) {
        int j = 0;
        do {
            row_sums[i] += matrix[i][j];
            j++;
        } while (j < size);
        i++;
    }
    
    printf("Row sums: ");
    for (int k = 0; k < size; k++) {
        printf("%d ", row_sums[k]);
    }
    printf("\n");
    
    return 0;
}