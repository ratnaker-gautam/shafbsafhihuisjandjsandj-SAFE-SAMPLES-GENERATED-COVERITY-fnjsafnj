//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int compress_chunk(uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size, int depth) {
    if (depth >= 10 || input_size == 0) {
        *output_size = 0;
        return 0;
    }
    
    if (input_size == 1) {
        output[0] = input[0];
        *output_size = 1;
        return 1;
    }
    
    size_t mid = input_size / 2;
    size_t left_size, right_size;
    
    if (mid > MAX_INPUT_SIZE || input_size - mid > MAX_INPUT_SIZE) {
        return -1;
    }
    
    int left_result = compress_chunk(input, mid, output, &left_size, depth + 1);
    if (left_result != 0) {
        return left_result;
    }
    
    int right_result = compress_chunk(input + mid, input_size - mid, output + left_size, &right_size, depth + 1);
    if (right_result != 0) {
        return right_result;
    }
    
    *output_size = left_size + right_size;
    return 0;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void print_buffer(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    Buffer input_buffer;
    Buffer output_buffer;
    
    printf("Enter input size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_buffer.size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    if (input_buffer.size == 0 || input_buffer.size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of range\n");
        return 1;
    }
    
    printf("Enter %zu bytes as hex values (space separated): ", input_buffer.size);
    for (size_t i = 0; i < input_buffer.size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            fprintf(stderr, "Invalid byte value\n");
            return 1;
        }
        input_buffer.data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    memset(output_buffer.data, 0, sizeof(output_buffer.data));
    
    int result = compress_chunk(input_buffer.data, input_buffer.size, 
                               output_buffer.data, &output_buffer.size, 0);
    
    if (result != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_buffer.size);
    print_buffer(input_buffer.data, input_buffer.size);
    
    printf("Compressed data (%zu bytes): ", output_buffer.size);
    print_buffer(output_buffer.data, output_buffer.size);
    
    printf("Compression ratio: %.2f%%\n", 
           (double)output_buffer.size / input_buffer.size * 100.0);
    
    return 0;
}