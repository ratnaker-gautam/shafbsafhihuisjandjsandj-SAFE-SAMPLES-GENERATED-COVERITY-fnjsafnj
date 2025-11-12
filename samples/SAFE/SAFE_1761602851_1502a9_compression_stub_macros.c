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

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t compressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += COMPRESSION_RATIO) {
        if (i + 1 < CHUNK_SIZE) {
            output[compressed_size] = (chunk[i] & MASK_BYTE) | ((chunk[i + 1] >> 4) & 0x0F);
            compressed_size++;
        } else {
            output[compressed_size] = chunk[i] & MASK_BYTE;
            compressed_size++;
        }
    }
    return compressed_size;
}

static int perform_compression(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    const uint8_t* current = input;
    
    while (remaining > 0) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        memcpy(chunk, current, chunk_size);
        
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO + 1];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        
        memcpy(&output->data[output->size], compressed_chunk, compressed_size);
        output->size += compressed_size;
        
        current += chunk_size;
        remaining -= chunk_size;
    }
    
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2 + 1];
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = 0;
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned long value;
        if (sscanf(token, "%02lx", &value) != 1) {
            fprintf(stderr, "Invalid input format\n");
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            fprintf(stderr, "Input value out of range\n");
            return EXIT_FAILURE;
        }
        input[input_size] = (uint8_t)value;
        input_size++;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input provided\n");
        return EXIT_FAILURE;
    }
    
    Buffer output;
    if (!perform_compression(input, input_size, &output)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    print_buffer(&output);
    return EXIT_SUCCESS;
}