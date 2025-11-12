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
    size_t output_index = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += 2) {
        if (i + 1 < CHUNK_SIZE) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[output_index++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[output_index++] = chunk[i] >> SHIFT_AMOUNT;
        }
    }
    return output_index;
}

static Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer result = {0};
    if (!validate_input(input, input_size)) return result;
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (input_size - i < CHUNK_SIZE) ? input_size - i : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = input[i + j];
        }
        
        uint8_t compressed[CHUNK_SIZE / COMPRESSION_RATIO];
        size_t compressed_size = compress_chunk(chunk, compressed);
        
        if (output_index + compressed_size > MAX_OUTPUT_SIZE) break;
        
        for (size_t k = 0; k < compressed_size; k++) {
            result.data[output_index++] = compressed[k];
        }
    }
    result.size = output_index;
    return result;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    char input_line[MAX_INPUT_SIZE * 2 + 1];
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0 || input_len == 0) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    size_t binary_size = input_len / 2;
    if (binary_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }
    
    uint8_t binary_input[MAX_INPUT_SIZE];
    for (size_t i = 0; i < binary_size; i++) {
        char hex_byte[3] = {input_line[i * 2], input_line[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex byte\n");
            return EXIT_FAILURE;
        }
        binary_input[i] = (uint8_t)value;
    }
    
    Buffer compressed = compress_data(binary_input, binary_size);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed: ");
    print_hex(compressed.data, compressed.size);
    
    return EXIT_SUCCESS;
}