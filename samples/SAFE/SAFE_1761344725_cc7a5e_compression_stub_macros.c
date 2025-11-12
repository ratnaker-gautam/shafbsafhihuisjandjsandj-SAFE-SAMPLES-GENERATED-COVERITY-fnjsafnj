//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define MASK_BYTE 0xAA

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

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || output == NULL) return 0;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        if (compressed_size >= MAX_OUTPUT_SIZE) break;
        output[compressed_size++] = chunk[i] ^ MASK_BYTE;
    }
    return compressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size && output->size < MAX_OUTPUT_SIZE) {
        size_t remaining = input_size - total_processed;
        size_t current_chunk = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        size_t compressed = compress_chunk(input + total_processed, current_chunk, 
                                         output->data + output->size);
        if (compressed == 0) break;
        
        output->size += compressed;
        total_processed += current_chunk;
    }
    
    return (total_processed == input_size);
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        if (i > 0) printf(" ");
        printf("%02X", data[i]);
    }
    printf("\n");
}

static int read_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) return 0;
    
    printf("Enter data to compress (hex bytes, space separated): ");
    fflush(stdout);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    *actual_size = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL && *actual_size < max_size) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) return 0;
        if (byte > 0xFF) return 0;
        
        buffer[(*actual_size)++] = (uint8_t)byte;
        token = strtok(NULL, " \t\n");
    }
    
    return (*actual_size > 0);
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer output;
    
    if (!read_input(input, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!compress_data(input, input_size, &output)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", output.size);
    printf("Compressed data: ");
    print_hex(output.data, output.size);
    
    return EXIT_SUCCESS;
}