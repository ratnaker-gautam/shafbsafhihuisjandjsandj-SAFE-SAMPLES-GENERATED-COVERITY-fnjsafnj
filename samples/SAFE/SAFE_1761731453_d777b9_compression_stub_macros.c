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
    if (chunk_size == 0 || output == NULL) return 0;
    
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] & BYTE_MASK;
            uint8_t second = chunk[i + 1] & BYTE_MASK;
            output[output_index++] = ((first >> SHIFT_AMOUNT) << SHIFT_AMOUNT) | (second >> SHIFT_AMOUNT);
        } else {
            output[output_index++] = (chunk[i] >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        }
    }
    return output_index;
}

static Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer result = {0};
    if (!validate_input(input, input_size)) return result;
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t remaining = input_size - i;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        if (total_compressed + (chunk_size / COMPRESSION_RATIO) >= MAX_OUTPUT_SIZE) {
            result.size = 0;
            return result;
        }
        
        size_t compressed_size = compress_chunk(input + i, chunk_size, 
                                              result.data + total_compressed);
        total_compressed += compressed_size;
    }
    
    result.size = total_compressed;
    return result;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 4 == 0) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

static int read_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter data to compress (up to %zu bytes as hex digits): ", max_size);
    
    char line[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
    
    size_t hex_len = strlen(line);
    if (hex_len % 2 != 0) return 0;
    if (hex_len / 2 > max_size) return 0;
    
    for (size_t i = 0; i < hex_len; i += 2) {
        char hex_byte[3] = {line[i], line[i + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr == hex_byte || *endptr != '\0') return 0;
        if (value < 0 || value > UINT8_MAX) return 0;
        buffer[i / 2] = (uint8_t)value;
    }
    
    return hex_len / 2;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = read_input(input_data, MAX_INPUT_SIZE);
    
    if (input_size == 0) {
        printf("Invalid input or no data provided.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    Buffer compressed = compress_data(input_data, input_size);
    if (compressed.size == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    printf("Compression ratio: %.2f:1\n", (double)input_size / compressed.size);
    
    return EXIT_SUCCESS;
}