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
    for (size_t i = 0; i < chunk_size && i < CHUNK_SIZE; i += 2) {
        if (i + 1 < chunk_size) {
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
        size_t remaining = input_size - i;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        if (output_index + COMPRESSION_RATIO >= MAX_OUTPUT_SIZE) break;
        
        size_t compressed_size = compress_chunk(&input[i], chunk_size, &result.data[output_index]);
        output_index += compressed_size;
    }
    
    result.size = output_index;
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
    
    printf("Enter data to compress (up to %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && total_read < max_size - 1) {
        buffer[total_read++] = (uint8_t)c;
    }
    buffer[total_read] = '\0';
    
    return total_read > 0 ? (int)total_read : 0;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE] = {0};
    
    int input_size = read_input(input, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "Error: No valid input provided\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed = compress_data(input, (size_t)input_size);
    if (compressed.size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f:1\n", (float)input_size / compressed.size);
    
    printf("Compressed data (hex):\n");
    print_hex(compressed.data, compressed.size);
    
    return EXIT_SUCCESS;
}