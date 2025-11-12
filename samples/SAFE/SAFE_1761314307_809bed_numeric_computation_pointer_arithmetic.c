//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < 1 || value > MAX_SIZE) return -1;
    
    *valid = (int)value;
    return 0;
}

void fill_array(int* arr, int size) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        *ptr = (int)(ptr - arr) + 1;
        ptr++;
    }
}

void compute_squares(int* src, int* dest, int size) {
    if (src == NULL || dest == NULL || size < 1 || size > MAX_SIZE) return;
    
    int* src_ptr = src;
    int* dest_ptr = dest;
    int* end = src + size;
    
    while (src_ptr < end) {
        long square = (long)(*src_ptr) * (long)(*src_ptr);
        if (square > 2147483647L) {
            *dest_ptr = 0;
        } else {
            *dest_ptr = (int)square;
        }
        src_ptr++;
        dest_ptr++;
    }
}

void compute_cubes(int* src, int* dest, int size) {
    if (src == NULL || dest == NULL || size < 1 || size > MAX_SIZE) return;
    
    int* src_ptr = src;
    int* dest_ptr = dest;
    int* end = src + size;
    
    while (src_ptr < end) {
        long cube = (long)(*src_ptr) * (long)(*src_ptr) * (long)(*src_ptr);
        if (cube > 2147483647L) {
            *dest_ptr = 0;
        } else {
            *dest_ptr = (int)cube;
        }
        src_ptr++;
        dest_ptr++;
    }
}

void print_results(int* original, int* squares, int* cubes, int size) {
    if (original == NULL || squares == NULL || cubes == NULL || size < 1 || size > MAX_SIZE) return;
    
    printf("Number\tSquare\tCube\n");
    printf("------\t------\t----\n");
    
    int* orig_ptr = original;
    int* sq_ptr = squares;
    int* cube_ptr = cubes;
    int* end = original + size;
    
    while (orig_ptr < end) {
        printf("%d\t%d\t%d\n", *orig_ptr, *sq_ptr, *cube_ptr);
        orig_ptr++;
        sq_ptr++;
        cube_ptr++;
    }
}

int main(void) {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int original[MAX_SIZE];
    int squares[MAX_SIZE];
    int cubes[MAX_SIZE];
    
    fill_array(original, size);
    compute_squares(original, squares, size);
    compute_cubes(original, cubes, size);
    print_results(original, squares, cubes, size);
    
    return 0;
}