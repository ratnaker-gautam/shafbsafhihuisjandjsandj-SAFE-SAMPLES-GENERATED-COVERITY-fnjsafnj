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

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    if (chunk == NULL || output == NULL) return 0;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += 2) {
        if (i + 1 < CHUNK_SIZE) {
            uint8_t first = chunk[i] & BYTE_MASK;
            uint8_t second = chunk[i + 1] & BYTE_MASK;
            output[compressed_size] = ((first >> SHIFT_AMOUNT) << SHIFT_AMOUNT) | (second >> SHIFT_AMOUNT);
            compressed_size++;
        }
    }
    return compressed_size;
}

static Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer result = {0};
    
    if (!validate_input(input, input_size)) {
        return result;
    }
    
    size_t output_index = 0;
    size_t remaining = input_size;
    
    while (remaining > 0 && output_index < MAX_OUTPUT_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        for (size_t i = 0; i < chunk_size; i++) {
            chunk[i] = input[input_size - remaining + i];
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        
        if (output_index + compressed_size > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (size_t i = 0; i < compressed_size; i++) {
            result.data[output_index++] = compressed_chunk[i];
        }
        
        if (remaining >= chunk_size) {
            remaining -= chunk_size;
        } else {
            break;
        }
    }
    
    result.size = output_index;
    return result;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (buffer->size > 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    char input_line[MAX_INPUT_SIZE * 3];
    
    printf("Enter data to compress (hex bytes, space separated): ");
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char* token = strtok(input_line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned long value;
        if (sscanf(token, "%02lX", &value) != 1) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            fprintf(stderr, "Byte value out of range: %s\n", token);
            return EXIT_FAILURE;
        }
        input_data[input_size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", input_data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
    
    Buffer compressed = compress_data(input_data, input_size);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer(&compressed);
    
    double ratio = (double)compressed.size / (double)input_size;
    printf("Compression ratio: %.2f\n", ratio);
    
    return EXIT_SUCCESS;
}