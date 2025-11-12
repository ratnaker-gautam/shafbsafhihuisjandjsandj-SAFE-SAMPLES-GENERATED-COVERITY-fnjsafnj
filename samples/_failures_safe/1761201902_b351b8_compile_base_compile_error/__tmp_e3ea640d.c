//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER + HEADER_SIZE)

#define RLE_THRESHOLD 3
#define MIN_REPEAT_COUNT 4
#define MAX_REPEAT_COUNT 255

#define COMPRESSION_SUCCESS 0
#define COMPRESSION_FAILURE 1

typedef struct {
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_COMPRESSED_SIZE];
} CompressionResult;

static size_t safe_read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0 || max_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t total_read = 0;
    int current_char;
    
    while (total_read < max_size - 1) {
        current_char = getchar();
        if (current_char == EOF || current_char == '\n') {
            break;
        }
        buffer[total_read++] = (uint8_t)current_char;
    }
    
    buffer[total_read] = '\0';
    return total_read;
}

static void write_header(uint8_t *output, uint32_t original, uint32_t compressed) {
    if (output == NULL) return;
    
    output[0] = (original >> 24) & 0xFF;
    output[1] = (original >> 16) & 0xFF;
    output[2] = (original >> 8) & 0xFF;
    output[3] = original & 0xFF;
    
    output[4] = (compressed >> 24) & 0xFF;
    output[5] = (compressed >> 16) & 0xFF;
    output[6] = (compressed >> 8) & 0xFF;
    output[7] = compressed & 0xFF;
}

static int simple_rle_compress(const uint8_t *input, size_t input_size, CompressionResult *result) {
    if (input == NULL || result == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return COMPRESSION_FAILURE;
    }
    
    size_t output_index = HEADER_SIZE;
    size_t input_index = 0;
    
    while (input_index < input_size) {
        uint8_t current = input[input_index];
        size_t count = 1;
        
        while (input_index + count < input_size && 
               input[input_index + count] == current && 
               count < MAX_REPEAT_COUNT) {
            count++;
        }
        
        if (count >= MIN_REPEAT_COUNT) {
            if (output_index + 2 > MAX_COMPRESSED_SIZE) {
                return COMPRESSION_FAILURE;
            }
            result->data[output_index++] = current;
            result->data[output_index++] = (uint8_t)count;
            input_index += count;
        } else {
            if (output_index + 1 > MAX_COMPRESSED_SIZE) {
                return COMPRESSION_FAILURE;
            }
            result->data[output_index++] = current;
            input_index++;
        }
    }
    
    if (output_index > MAX_COMPRESSED_SIZE) {
        return COMPRESSION_FAILURE;
    }
    
    result->original_size = (uint32_t)input_size;
    result->compressed_size = (uint32_t)output_index;
    write_header(result->data, result->original_size, result->compressed_size);
    
    return COMPRESSION_SUCCESS;
}

static void print_compressed_data(const CompressionResult *result) {
    if (result == NULL || result->compressed_size == 0 || 
        result->compressed_size > MAX_COMPRESSED_SIZE) {
        return;
    }
    
    printf("Compressed data (%u bytes -> %u bytes):\n", 
           result->original_size, result->compressed_size);
    
    for (size_t i = 0; i < result->compressed_size; i++) {
        printf("%02X", result->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    
    if (result->compressed_size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    CompressionResult result;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    size_t input_size = safe_read_input(input_buffer, sizeof(input_buffer));
    if (input_size == 0) {
        printf("Error: No input received or input too large\n");
        return EXIT_FAILURE;
    }
    
    printf("Original text (%zu bytes): %s\n", input_size, input_buffer);
    
    memset(&result, 0, sizeof(result));
    
    if (simple_rle_compress(input_buffer, input_size, &result) == COMPRESSION_SUCCESS) {
        print_compressed_data(&result);
        
        if (result.compressed_size < result.original_size) {
            double ratio = (double)result.compressed