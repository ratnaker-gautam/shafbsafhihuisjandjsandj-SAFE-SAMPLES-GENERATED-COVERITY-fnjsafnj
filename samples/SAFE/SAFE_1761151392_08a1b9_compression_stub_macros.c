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

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[output_size++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[output_size++] = chunk[i] >> SHIFT_AMOUNT;
        }
    }
    return output_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t remaining = input_size - i;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        if (total_compressed + COMPRESSION_RATIO > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        size_t compressed_size = compress_chunk(input + i, chunk_size, 
                                              output->data + total_compressed);
        total_compressed += compressed_size;
    }
    
    output->size = total_compressed;
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
    }
    printf("\n");
}

static int read_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) return 0;
    
    char input_line[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return 0;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0) {
        return 0;
    }
    
    size_t byte_count = input_len / 2;
    if (byte_count > max_size) {
        return 0;
    }
    
    for (size_t i = 0; i < byte_count; i++) {
        char hex_byte[3] = {input_line[i * 2], input_line[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            return 0;
        }
        buffer[i] = (uint8_t)value;
    }
    
    *actual_size = byte_count;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer compressed_data;
    
    printf("Enter input data as hex bytes (e.g., A1B2C3): ");
    if (!read_input(input_data, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error: Invalid input format or size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Error: No input data provided\n");
        return EXIT_FAILURE;
    }
    
    if (!compress_data(input_data, input_size, &compressed_data)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data: ");
    print_buffer(&compressed_data);
    
    return EXIT_SUCCESS;
}