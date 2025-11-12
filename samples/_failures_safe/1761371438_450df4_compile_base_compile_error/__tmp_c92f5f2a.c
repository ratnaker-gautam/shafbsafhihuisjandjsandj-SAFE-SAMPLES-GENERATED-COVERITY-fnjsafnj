//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)

#define RLE_THRESHOLD 3
#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)

#define CHECK_BOUNDS(offset, max, size) \
    do { \
        if ((offset) + (size) > (max)) { \
            fprintf(stderr, "Error: Buffer overflow\n"); \
            return EXIT_FAILURE; \
        } \
    } while(0)

#define WRITE_BYTE(dest, offset, value) \
    do { \
        CHECK_BOUNDS(offset, MAX_OUTPUT_SIZE, 1); \
        dest[offset++] = (value); \
    } while(0)

static size_t compress_rle(const uint8_t* input, size_t input_len, uint8_t* output) {
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count >= RLE_THRESHOLD) {
            WRITE_BYTE(output, out_pos, 0xFF);
            WRITE_BYTE(output, out_pos, current);
            WRITE_BYTE(output, out_pos, (uint8_t)count);
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (current == 0xFF) {
                    WRITE_BYTE(output, out_pos, 0xFE);
                }
                WRITE_BYTE(output, out_pos, current);
            }
            i += count;
        }
    }
    
    return out_pos;
}

static size_t decompress_rle(const uint8_t* input, size_t input_len, uint8_t* output) {
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i++];
        
        if (current == 0xFF && i + 1 < input_len) {
            uint8_t value = input[i++];
            uint8_t count = input[i++];
            
            CHECK_BOUNDS(out_pos, MAX_INPUT_SIZE, count);
            for (uint8_t j = 0; j < count; j++) {
                output[out_pos++] = value;
            }
        } else if (current == 0xFE && i < input_len) {
            WRITE_BYTE(output, out_pos, 0xFF);
            i++;
        } else {
            WRITE_BYTE(output, out_pos, current);
        }
    }
    
    return out_pos;
}

static int validate_input(const uint8_t* data, size_t len) {
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!IS_PRINTABLE(data[i]) && data[i] != '\n' && data[i] != '\t') {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input data\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input_buffer, input_len, compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_len || memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / input_len * 100.0f);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");