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
    for (size_t i = 0; i < CHUNK_SIZE; i += 2) {
        if (i + 1 < CHUNK_SIZE) {
            uint16_t combined = (chunk[i] << 8) | chunk[i + 1];
            output[compressed_size++] = (combined >> 6) & BYTE_MASK;
            output[compressed_size++] = combined & 0x3F;
        } else {
            output[compressed_size++] = chunk[i];
        }
    }
    return compressed_size;
}

static int perform_compression(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    const uint8_t* current = input;
    
    while (remaining > 0 && output->size < MAX_OUTPUT_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        memcpy(chunk, current, chunk_size);
        
        uint8_t compressed_chunk[CHUNK_SIZE];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
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

static int read_input(Buffer* input) {
    char line[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    
    if (len % 2 != 0 || len == 0) {
        return 0;
    }
    
    input->size = len / 2;
    if (input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < input->size; i++) {
        char hex[3] = {line[i * 2], line[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) {
            return 0;
        }
        input->data[i] = (uint8_t)value;
    }
    
    return 1;
}

int main(void) {
    Buffer input_data = {0};
    Buffer output_data = {0};
    
    printf("Enter input data as hex string: ");
    if (!read_input(&input_data)) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!perform_compression(input_data.data, input_data.size, &output_data)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data: ");
    print_buffer(&output_data);
    
    return EXIT_SUCCESS;
}