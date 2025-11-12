//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MARKER 0xFF
#define MAX_RUN_LENGTH 255
#define MIN_RUN_LENGTH 3

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

#define BUFFER_INIT {NULL, 0, 0}
#define BUFFER_GROW(buffer, min_capacity) do { \
    size_t new_cap = (buffer).capacity ? (buffer).capacity * 2 : 64; \
    if (new_cap < (min_capacity)) new_cap = (min_capacity); \
    uint8_t *new_data = realloc((buffer).data, new_cap); \
    if (!new_data) { perror("Memory allocation failed"); exit(EXIT_FAILURE); } \
    (buffer).data = new_data; \
    (buffer).capacity = new_cap; \
} while(0)

#define BUFFER_APPEND(buffer, byte) do { \
    if ((buffer).size >= (buffer).capacity) BUFFER_GROW(buffer, (buffer).size + 1); \
    (buffer).data[(buffer).size++] = (byte); \
} while(0)

static void compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && run_length < MAX_RUN_LENGTH && 
               input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= MIN_RUN_LENGTH) {
            BUFFER_APPEND(*output, COMPRESSED_MARKER);
            BUFFER_APPEND(*output, current);
            BUFFER_APPEND(*output, (uint8_t)run_length);
            i += run_length;
        } else {
            BUFFER_APPEND(*output, current);
            i++;
        }
    }
}

static int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_size) {
                fprintf(stderr, "Invalid compressed data: truncated run\n");
                return -1;
            }
            
            uint8_t value = input[i + 1];
            uint8_t run_length = input[i + 2];
            
            if (run_length < MIN_RUN_LENGTH) {
                fprintf(stderr, "Invalid compressed data: run length too short\n");
                return -1;
            }
            
            for (uint8_t j = 0; j < run_length; j++) {
                BUFFER_APPEND(*output, value);
            }
            
            i += 3;
        } else {
            BUFFER_APPEND(*output, input[i]);
            i++;
        }
    }
    return 0;
}

static void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed = BUFFER_INIT;
    compress_rle((uint8_t*)input_str, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100);
    
    printf("Compressed data (hex):\n");
    print_hex(compressed.data, compressed.size);
    
    Buffer decompressed = BUFFER_INIT;
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);
        free(decompressed.data);
        return EXIT_FAILURE;
    }
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input_str, input_len) != 0) {
        fprintf(st