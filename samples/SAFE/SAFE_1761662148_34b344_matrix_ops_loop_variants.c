//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void fill_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int value = 1;
    for (int i = 0; i < size; i++) {
        int j = 0;
        while (j < size) {
            matrix[i][j] = value++;
            j++;
        }
    }
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    int temp;
    int i = 0;
    do {
        int j = i + 1;
        while (j < size) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
            j++;
        }
        i++;
    } while (i < size);
}

int get_valid_size(void) {
    char buffer[32];
    int size;
    
    while (1) {
        printf("Enter matrix size (1-%d): ", MAX_SIZE);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char *endptr;
        long val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (val < 1 || val > MAX_SIZE) {
            printf("Size must be between 1 and %d.\n", MAX_SIZE);
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
    if (size == -1) {
        return 1;
    }
    
    printf("\nOriginal matrix:\n");
    fill_matrix(matrix, size);
    print_matrix(matrix, size);
    
    printf("\nTransposed matrix:\n");
    transpose_matrix(matrix, size);
    print_matrix(matrix, size);
    
    printf("\nMatrix sum by rows:\n");
    for (int i = 0; i < size; i++) {
        int sum = 0;
        int j = 0;
        do {
            sum += matrix[i][j];
            j++;
        } while (j < size);
        printf("Row %d sum: %d\n", i, sum);
    }
    
    printf("\nMatrix sum by columns:\n");
    for (int j = 0; j < size; j++) {
        int sum = 0;
        int i = 0;
        while (i < size) {
            sum += matrix[i][j];
            i++;
        }
        printf("Column %d sum: %d\n", j, sum);
    }
    
    return 0;
}