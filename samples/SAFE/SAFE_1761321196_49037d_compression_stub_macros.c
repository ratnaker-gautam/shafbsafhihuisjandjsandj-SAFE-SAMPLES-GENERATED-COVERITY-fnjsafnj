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
#define ESCAPE_CHAR 0xFF

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
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < chunk_size) {
        uint8_t current = chunk[i];
        size_t count = 1;
        
        while (i + count < chunk_size && chunk[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (out_pos + 3 > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = ESCAPE_CHAR;
            output[out_pos++] = (uint8_t)count;
            output[out_pos++] = current;
        } else {
            if (out_pos + count > MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[out_pos++] = current;
            }
        }
        
        i += count;
    }
    
    return out_pos;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        
        uint8_t chunk_output[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input + total_processed, chunk_size, chunk_output);
        
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        
        memcpy(output->data + output->size, chunk_output, compressed_size);
        output->size += compressed_size;
        total_processed += chunk_size;
    }
    
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes of data (hex values 00-FF): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            fprintf(stderr, "Invalid data format\n");
            return EXIT_FAILURE;
        }
        if (value > 0xFF) {
            fprintf(stderr, "Value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    Buffer compressed;
    if (!compress_data(input_data, input_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    printf("Compressed data:\n");
    print_hex(compressed.data, compressed.size);
    
    return EXIT_SUCCESS;
}