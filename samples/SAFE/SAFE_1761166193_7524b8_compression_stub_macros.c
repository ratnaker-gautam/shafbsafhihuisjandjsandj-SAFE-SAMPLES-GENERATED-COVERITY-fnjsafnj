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
    if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
    
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] & BYTE_MASK;
            uint8_t second = chunk[i + 1] & BYTE_MASK;
            output[output_size] = ((first >> SHIFT_AMOUNT) << SHIFT_AMOUNT) | (second >> SHIFT_AMOUNT);
            output_size++;
        } else {
            output[output_size] = (chunk[i] >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
            output_size++;
        }
    }
    return output_size;
}

static Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer result = {0};
    if (!validate_input(input, input_size)) return result;
    
    size_t output_pos = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i < CHUNK_SIZE) ? input_size - i : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(&input[i], chunk_size, &result.data[output_pos]);
        
        if (output_pos + compressed_size >= MAX_OUTPUT_SIZE) {
            result.size = 0;
            return result;
        }
        
        output_pos += compressed_size;
    }
    result.size = output_pos;
    return result;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed = compress_data((uint8_t*)input_str, input_len);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f:1\n", (float)input_len / compressed.size);
    printf("Compressed data (hex): ");
    print_hex(compressed.data, compressed.size);
    
    return EXIT_SUCCESS;
}