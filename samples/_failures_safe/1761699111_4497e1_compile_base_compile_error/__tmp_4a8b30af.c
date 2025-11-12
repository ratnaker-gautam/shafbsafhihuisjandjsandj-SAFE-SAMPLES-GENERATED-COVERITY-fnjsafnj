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
    size_t new_capacity = (buffer).capacity * 2; \
    if (new_capacity < min_capacity) new_capacity = min_capacity; \
    if (new_capacity < 16) new_capacity = 16; \
    uint8_t *new_data = realloc((buffer).data, new_capacity); \
    if (!new_data) { perror("Memory allocation failed"); exit(EXIT_FAILURE); } \
    (buffer).data = new_data; \
    (buffer).capacity = new_capacity; \
} while(0)

#define BUFFER_APPEND(buffer, byte) do { \
    if ((buffer).size >= (buffer).capacity) { \
        BUFFER_GROW((buffer), (buffer).size + 1); \
    } \
    (buffer).data[(buffer).size++] = (byte); \
} while(0)

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size > MAX_INPUT_SIZE) {
        return -1;
    }

    output->size = 0;
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
    
    return 0;
}

static int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output) {
        return -1;
    }

    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_size) {
                return -1;
            }
            
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH) {
                return -1;
            }
            
            for (uint8_t j = 0; j < count; j++) {
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

static void cleanup_buffer(Buffer *buffer) {
    if (buffer && buffer->data) {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
        buffer->capacity = 0;
    }
}

int main(void) {
    Buffer input_buffer = BUFFER_INIT;
    Buffer compressed_buffer = BUFFER_INIT;
    Buffer decompressed_buffer = BUFFER_INIT;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_buffer.size >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            cleanup_buffer(&input_buffer);
            cleanup_buffer(&compressed_buffer);
            cleanup_buffer(&decompressed_buffer);
            return EXIT_FAILURE;
        }
        BUFFER_APPEND(input_buffer, (uint8_t)c);
    }
    
    if (input_buffer.size == 0) {
        printf("No input provided\n");
        cleanup_buffer(&input_buffer);
        cleanup_buffer(&compressed_buffer);
        cleanup_buffer(&decompressed_buffer);
        return EXIT_FAILURE;
    }
    
    if (compress_rle(input_buffer.data, input_buffer.size, &compressed_buffer) != 0) {
        printf("Compression failed\n");
        cleanup_buffer(&input_buffer);
        cleanup_buffer(&compressed_buffer);
        cleanup_buffer(&decompressed_buffer);
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_buffer.size);
    printf("Compressed size: %zu bytes\n", compressed_buffer.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buffer.size / input_buffer.size) * 100);
    
    if (decompress_rle(compressed_buffer.data, compressed_buffer.size, &decompressed_buffer) != 0) {
        printf("Decompression failed\n");
        cleanup_buffer(&input_buffer);
        cleanup_buffer(&compressed_buffer);
        cleanup_buffer(&decompressed_buffer);
        return EXIT