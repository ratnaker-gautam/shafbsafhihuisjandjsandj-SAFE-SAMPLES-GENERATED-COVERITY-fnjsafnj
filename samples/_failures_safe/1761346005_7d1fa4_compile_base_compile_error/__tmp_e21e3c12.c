//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t compressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += 2) {
        if (i + 1 < CHUNK_SIZE) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[compressed_size++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[compressed_size++] = chunk[i] >> SHIFT_AMOUNT;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t decompressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE / 2; i++) {
        uint8_t compressed = chunk[i];
        output[decompressed_size++] = (compressed >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        output[decompressed_size++] = (compressed & (BYTE_MASK >> SHIFT_AMOUNT)) << SHIFT_AMOUNT;
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (i + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : input_size - i;
        memcpy(chunk, input + i, chunk_size);
        
        uint8_t compressed_chunk[CHUNK_SIZE];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        memcpy(output->data + output->size, compressed_chunk, compressed_size);
        output->size += compressed_size;
    }
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE / 2) {
        uint8_t chunk[CHUNK_SIZE / 2] = {0};
        size_t chunk_size = (i + CHUNK_SIZE / 2 <= input_size) ? CHUNK_SIZE / 2 : input_size - i;
        memcpy(chunk, input + i, chunk_size);
        
        uint8_t decompressed_chunk[CHUNK_SIZE];
        size_t decompressed_size = decompress_chunk(chunk, decompressed_chunk);
        
        if (output->size + decompressed_size > MAX_OUTPUT_SIZE) return 0;
        memcpy(output->data + output->size, decompressed_chunk, decompressed_size);
        output->size += decompressed_size;
    }
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

static int hex_char_to_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data to compress (hex characters, max %d bytes): ", MAX_INPUT_SIZE * 2);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(hex_input);
    if (input_len > 0 && hex_input[input