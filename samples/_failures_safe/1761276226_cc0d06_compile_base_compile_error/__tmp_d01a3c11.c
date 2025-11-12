//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER)
#define RUN_LENGTH_MAX 255
#define MIN_COMPRESSIBLE_RUN 3

typedef struct {
    uint8_t data[MAX_COMPRESSED_SIZE];
    size_t size;
} CompressedData;

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} InputData;

static int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_max == 0) return 0;
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output_index + 2 > output_max) return 0;
        
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && run_length < RUN_LENGTH_MAX && input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= MIN_COMPRESSIBLE_RUN) {
            output[output_index++] = current;
            output[output_index++] = (uint8_t)run_length;
            i += run_length;
        } else {
            size_t literal_count = 0;
            while (i + literal_count < input_size && literal_count < RUN_LENGTH_MAX && 
                   (literal_count + 1 >= input_size || input[i + literal_count] != input[i + literal_count + 1] || 
                    (i + literal_count + 2 < input_size && input[i + literal_count] == input[i + literal_count + 1] && 
                     input[i + literal_count] == input[i + literal_count + 2]))) {
                literal_count++;
                if (i + literal_count < input_size && literal_count < RUN_LENGTH_MAX && 
                    input[i + literal_count] == input[i + literal_count - 1] && 
                    (i + literal_count + 1 >= input_size || input[i + literal_count] != input[i + literal_count + 1])) {
                    break;
                }
            }
            
            if (output_index + literal_count + 1 > output_max) return 0;
            
            output[output_index++] = 0xFF;
            output[output_index++] = (uint8_t)literal_count;
            
            for (size_t j = 0; j < literal_count; j++) {
                if (output_index >= output_max) return 0;
                output[output_index++] = input[i + j];
            }
            i += literal_count;
        }
    }
    
    return output_index;
}

static size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_max == 0) return 0;
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF) {
            if (i + 1 >= input_size) return 0;
            
            uint8_t literal_count = input[i + 1];
            i += 2;
            
            if (i + literal_count > input_size) return 0;
            if (output_index + literal_count > output_max) return 0;
            
            for (uint8_t j = 0; j < literal_count; j++) {
                output[output_index++] = input[i + j];
            }
            i += literal_count;
        } else {
            if (i + 1 >= input_size) return 0;
            
            uint8_t value = input[i];
            uint8_t count = input[i + 1];
            i += 2;
            
            if (output_index + count > output_max) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_index++] = value;
            }
        }
    }
    
    return output_index;
}

static int compare_data(const uint8_t* a, const uint8_t* b, size_t size) {
    if (a == NULL || b == NULL) return 0;
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    InputData original;
    CompressedData compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &original.size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (original.size == 0 || original.size > MAX_INPUT_SIZE) {
        printf("Invalid size