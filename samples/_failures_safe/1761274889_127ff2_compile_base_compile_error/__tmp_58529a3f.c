//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define CHUNK_SIZE 16
#define COMPRESSION_THRESHOLD 3
#define RUN_LENGTH_MARKER 0xFF

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
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && 
               input[i + count] == current && count < COMPRESSION_THRESHOLD + UINT8_MAX) {
            count++;
        }
        
        if (count >= COMPRESSION_THRESHOLD) {
            if (out_idx + 3 >= MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = RUN_LENGTH_MARKER;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            if (out_idx + count >= MAX_OUTPUT_SIZE) return 0;
            for (size_t j = 0; j < count; j++) {
                output->data[out_idx++] = input[i + j];
            }
            i += count;
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static size_t decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == RUN_LENGTH_MARKER) {
            if (i + 2 >= input_size) return 0;
            
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count >= MAX_OUTPUT_SIZE) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
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

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        if (i > 0 && i % CHUNK_SIZE == 0) {
            putchar('\n');
        }
        printf("%02X ", data[i]);
    }
    putchar('\n');
}

static int get_user_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter data to compress (up to %zu bytes as hex, space separated): ", max_size);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    size_t idx = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL && idx < max_size) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) != 1) return 0;
        if (value > UINT8_MAX) return 0;
        buffer[idx++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    return (int)idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    int input_size = get_user_input(input_data, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original data (%d bytes):\n", input_size);
    print_hex(input_data, (size_t)input_size);
    
    size_t compressed_size = compress_rle(input_data, (size_t)input_size, &compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    size_t decompressed_size = decompress_r