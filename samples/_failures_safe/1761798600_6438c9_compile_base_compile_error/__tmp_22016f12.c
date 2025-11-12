//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define ROTATE_BITS 3

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) {
        return SIZE_MAX;
    }
    return a + b;
}

static size_t safe_multiply(size_t a, size_t b) {
    if (a > 0 && b > SIZE_MAX / a) {
        return SIZE_MAX;
    }
    return a * b;
}

static uint8_t rotate_left(uint8_t value, int shift) {
    shift = shift % 8;
    if (shift < 0) shift += 8;
    return (value << shift) | (value >> (8 - shift));
}

static size_t compress_data(const uint8_t* input, size_t input_len, uint8_t* output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }

    size_t output_index = 0;
    size_t max_output = safe_multiply(input_len, COMPRESSED_MULTIPLIER);
    if (max_output == SIZE_MAX) {
        return 0;
    }

    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - i;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = rotate_left(input[i + j], ROTATE_BITS);
        }
        
        for (size_t j = 0; j < chunk_size; j += 2) {
            if (output_index >= max_output) {
                return output_index;
            }
            if (j + 1 < chunk_size) {
                uint8_t compressed = ((chunk[j] & 0xF0) | (chunk[j + 1] >> 4));
                output[output_index++] = compressed;
            } else {
                output[output_index++] = chunk[j] & 0xF0;
            }
        }
    }
    
    return output_index;
}

static size_t decompress_data(const uint8_t* input, size_t input_len, uint8_t* output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }

    size_t output_index = 0;
    size_t max_output = safe_multiply(input_len, COMPRESSED_MULTIPLIER);
    if (max_output == SIZE_MAX) {
        return 0;
    }

    for (size_t i = 0; i < input_len; i++) {
        if (output_index >= max_output) {
            break;
        }
        uint8_t compressed = input[i];
        uint8_t high_nibble = compressed & 0xF0;
        uint8_t low_nibble = (compressed & 0x0F) << 4;
        
        uint8_t byte1 = rotate_left(high_nibble, 8 - ROTATE_BITS);
        output[output_index++] = byte1;
        
        if (low_nibble != 0 && output_index < max_output) {
            uint8_t byte2 = rotate_left(low_nibble, 8 - ROTATE_BITS);
            output[output_index++] = byte2;
        }
    }
    
    return output_index;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_data(input_buffer, input_len, compressed_buffer);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size >= input_len) {
        printf("Compression ratio: %.2f%% (expansion)\n", 
               ((float)compressed_size / input_len - 1.0f) * 100.0f);
    } else {
        printf("Compression ratio: %.2f%%\n", 
               (1.0f - (float)compressed_size / input_len) * 100.0f