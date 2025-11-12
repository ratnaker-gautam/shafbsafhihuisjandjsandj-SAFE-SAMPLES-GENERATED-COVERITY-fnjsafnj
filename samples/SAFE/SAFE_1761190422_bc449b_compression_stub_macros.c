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
#define SHIFT_AMOUNT 3

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) {
        return SIZE_MAX;
    }
    return a + b;
}

static size_t safe_multiply(size_t a, size_t b) {
    if (b > 0 && a > SIZE_MAX / b) {
        return SIZE_MAX;
    }
    return a * b;
}

static int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || input == NULL || output == NULL) return 0;
    
    size_t out_index = 0;
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        size_t remaining = len - i;
        size_t chunk_len = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        uint8_t header = (uint8_t)chunk_len;
        output[out_index++] = header;
        
        for (size_t j = 0; j < chunk_len; j++) {
            uint8_t compressed = (input[i + j] >> SHIFT_AMOUNT) & BYTE_MASK;
            output[out_index++] = compressed;
        }
    }
    return (int)out_index;
}

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || input == NULL || output == NULL) return 0;
    
    size_t out_index = 0;
    size_t in_index = 0;
    
    while (in_index < len) {
        if (in_index >= len) break;
        uint8_t header = input[in_index++];
        size_t chunk_len = header;
        
        if (chunk_len == 0 || chunk_len > CHUNK_SIZE) break;
        if (in_index + chunk_len > len) break;
        
        for (size_t j = 0; j < chunk_len; j++) {
            uint8_t compressed = input[in_index++];
            output[out_index++] = (compressed << SHIFT_AMOUNT) & BYTE_MASK;
        }
    }
    return (int)out_index;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char line_buffer[MAX_INPUT_SIZE + 2];
    if (fgets(line_buffer, sizeof(line_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(line_buffer);
    if (input_len > 0 && line_buffer[input_len - 1] == '\n') {
        line_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }
    
    memcpy(input_buffer, line_buffer, input_len);
    
    int compressed_size = compress_chunk(input_buffer, input_len, compressed_buffer);
    if (compressed_size <= 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    int decompressed_size = decompress_chunk(compressed_buffer, (size_t)compressed_size, decompressed_buffer);
    if (decompressed_size <= 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if ((size_t)decompressed_size != input_len || memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %d bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / input_len * 100.0f);
    printf("Decompressed successfully: ");
    fwrite(decompressed_buffer, 1, (size_t)decompressed_size, stdout);
    printf("\n");
    
    return EXIT_SUCCESS;
}