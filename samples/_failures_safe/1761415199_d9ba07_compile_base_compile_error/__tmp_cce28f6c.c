//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 8

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0) return 0;
    if (len > MAX_INPUT_SIZE) return 0;
    return 1;
}

Buffer compress_data(const char *input, size_t len) {
    Buffer result = {NULL, 0};
    if (!validate_input(input, len)) return result;
    
    size_t compressed_size = (len + CHUNK_SIZE - 1) / CHUNK_SIZE * COMPRESSED_MULTIPLIER;
    if (compressed_size > SIZE_MAX / sizeof(uint8_t)) return result;
    
    result.data = malloc(compressed_size * sizeof(uint8_t));
    if (result.data == NULL) return result;
    result.size = compressed_size;
    
    for (size_t i = 0; i < compressed_size; i += COMPRESSED_MULTIPLIER) {
        size_t input_idx = i / COMPRESSED_MULTIPLIER * CHUNK_SIZE;
        uint16_t compressed_value = 0;
        
        for (int j = 0; j < CHUNK_SIZE && input_idx + j < len; j++) {
            compressed_value = (compressed_value << 1) | ((input[input_idx + j] & 1));
        }
        
        if (i < compressed_size) {
            result.data[i] = (compressed_value >> SHIFT_AMOUNT) & BYTE_MASK;
        }
        if (i + 1 < compressed_size) {
            result.data[i + 1] = compressed_value & BYTE_MASK;
        }
    }
    
    return result;
}

Buffer decompress_data(const uint8_t *compressed, size_t compressed_len, size_t original_len) {
    Buffer result = {NULL, 0};
    if (compressed == NULL) return result;
    if (compressed_len == 0) return result;
    if (original_len == 0) return result;
    if (original_len > MAX_INPUT_SIZE) return result;
    
    result.data = malloc(original_len * sizeof(uint8_t));
    if (result.data == NULL) return result;
    result.size = original_len;
    memset(result.data, 'X', original_len);
    
    for (size_t i = 0; i < compressed_len; i += COMPRESSED_MULTIPLIER) {
        size_t output_idx = i / COMPRESSED_MULTIPLIER * CHUNK_SIZE;
        uint16_t compressed_value = 0;
        
        if (i < compressed_len) {
            compressed_value = (compressed_value << SHIFT_AMOUNT) | compressed[i];
        }
        if (i + 1 < compressed_len) {
            compressed_value = (compressed_value << SHIFT_AMOUNT) | compressed[i + 1];
        }
        
        for (int j = CHUNK_SIZE - 1; j >= 0 && output_idx + j < original_len; j--) {
            uint8_t bit = (compressed_value >> j) & 1;
            result.data[output_idx + j] = bit ? '1' : '0';
        }
    }
    
    return result;
}

void print_buffer(const Buffer *buf, const char *label) {
    if (buf == NULL || buf->data == NULL) return;
    printf("%s (%zu bytes): ", label, buf->size);
    for (size_t i = 0; i < buf->size && i < 32; i++) {
        printf("%02X ", buf->data[i]);
    }
    if (buf->size > 32) printf("...");
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", len);
    
    Buffer compressed = compress_data(input, len);
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    print_buffer(&compressed, "Compressed");
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed.size / len * 100.0);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size, len);
    if (decompressed.data == NULL) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);