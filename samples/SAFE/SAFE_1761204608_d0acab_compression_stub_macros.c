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

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE];
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = input[i + j];
        }
        for (size_t j = chunk_size; j < CHUNK_SIZE; j++) {
            chunk[j] = 0;
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO + 1];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        
        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        for (size_t j = 0; j < compressed_size; j++) {
            output->data[total_compressed + j] = compressed_chunk[j];
        }
        total_compressed += compressed_size;
    }
    
    output->size = total_compressed;
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    char input_line[MAX_INPUT_SIZE * 2 + 1];
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0) {
        return EXIT_FAILURE;
    }
    
    size_t data_size = input_len / 2;
    if (data_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {input_line[i * 2], input_line[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    Buffer compressed;
    if (!compress_data(input_data, data_size, &compressed)) {
        return EXIT_FAILURE;
    }
    
    print_hex(compressed.data, compressed.size);
    
    return EXIT_SUCCESS;
}