//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR 0xFF
#define MAX_RUN_LENGTH 255

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && 
               input[i + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (out_idx + 3 > MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = ESCAPE_CHAR;
            output->data[out_idx++] = (uint8_t)run_length;
            output->data[out_idx++] = current;
        } else {
            if (out_idx + run_length > MAX_OUTPUT_SIZE) return 0;
            for (size_t j = 0; j < run_length; j++) {
                output->data[out_idx++] = current;
            }
        }
        
        i += run_length;
    }
    
    output->size = out_idx;
    return out_idx;
}

static size_t decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_size) return 0;
            
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (out_idx + run_length > MAX_OUTPUT_SIZE) return 0;
            
            for (uint8_t j = 0; j < run_length; j++) {
                output->data[out_idx++] = value;
            }
            
            i += 3;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = input[i++];
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

static void generate_test_data(Buffer* buf) {
    if (buf == NULL) return;
    
    const char* test_pattern = "AAAABBBBCCCDDDDEEEEFFFF";
    size_t len = strlen(test_pattern);
    
    if (len > MAX_INPUT_SIZE) len = MAX_INPUT_SIZE;
    
    for (size_t i = 0; i < len; i++) {
        buf->data[i] = (uint8_t)test_pattern[i];
    }
    buf->size = len;
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    generate_test_data(&input_data);
    
    printf("Original data (%zu bytes): ", input_data.size);
    print_buffer_hex(&input_data);
    
    if (compress_rle(input_data.data, input_data.size, &compressed) == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(&compressed);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (input_data.size != decompressed.size) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data.data, decompressed.data, input_data.size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression test passed successfully\n");
    return EXIT_SUCCESS;
}