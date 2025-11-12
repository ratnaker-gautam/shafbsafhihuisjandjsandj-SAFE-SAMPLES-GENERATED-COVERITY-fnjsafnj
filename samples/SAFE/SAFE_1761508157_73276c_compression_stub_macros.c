//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

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
    if (chunk_size == 0) return 0;
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i += COMPRESSION_FACTOR) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] & BYTE_MASK;
            uint8_t second = chunk[i + 1] & BYTE_MASK;
            output[output_size] = ((first >> SHIFT_AMOUNT) << SHIFT_AMOUNT) | (second >> SHIFT_AMOUNT);
            output_size++;
        } else {
            output[output_size] = (chunk[i] >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
            output_size++;
        }
        if (output_size >= MAX_OUTPUT_SIZE) break;
    }
    return output_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size && output->size < MAX_OUTPUT_SIZE) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        size_t compressed_size = compress_chunk(input + total_processed, chunk_size, 
                                              output->data + output->size);
        if (compressed_size == 0) break;
        
        output->size += compressed_size;
        total_processed += chunk_size;
    }
    
    return (total_processed == input_size);
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

static int read_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) return 0;
    
    printf("Enter data to compress (hex bytes, space separated, max %zu bytes): ", max_size);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    *actual_size = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL && *actual_size < max_size) {
        unsigned long value;
        if (sscanf(token, "%2lx", &value) != 1) break;
        if (value > UINT8_MAX) break;
        buffer[*actual_size] = (uint8_t)value;
        (*actual_size)++;
        token = strtok(NULL, " \t\n");
    }
    
    return (*actual_size > 0);
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer compressed_data;
    
    if (!read_input(input_data, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error: Invalid input or no data provided\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    if (!compress_data(input_data, input_size, &compressed_data)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes, %.1f%% of original):\n", 
           compressed_data.size, 
           (input_size > 0) ? (compressed_data.size * 100.0 / input_size) : 0.0);
    print_buffer(&compressed_data);
    
    return EXIT_SUCCESS;
}