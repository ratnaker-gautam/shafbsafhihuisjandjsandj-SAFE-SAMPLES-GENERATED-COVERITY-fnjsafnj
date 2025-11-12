//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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
#define BUFFER_GROW(buffer, min_capacity) \
    do { \
        size_t new_capacity = (buffer).capacity ? (buffer).capacity * 2 : 64; \
        if (new_capacity < (min_capacity)) new_capacity = (min_capacity); \
        uint8_t *new_data = realloc((buffer).data, new_capacity); \
        if (!new_data) { \
            free((buffer).data); \
            exit(EXIT_FAILURE); \
        } \
        (buffer).data = new_data; \
        (buffer).capacity = new_capacity; \
    } while(0)

#define BUFFER_APPEND(buffer, byte) \
    do { \
        if ((buffer).size >= (buffer).capacity) { \
            BUFFER_GROW(buffer, (buffer).size + 1); \
        } \
        (buffer).data[(buffer).size++] = (byte); \
    } while(0)

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size == 0) return -1;
    
    output->size = 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && 
               input[i + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
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
    if (!input || !output || input_size == 0) return -1;
    
    output->size = 0;
    
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_size) return -1;
            
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH) return -1;
            
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

static void cleanup_buffer(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int main(void) {
    Buffer input_data = BUFFER_INIT;
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char input_buffer[MAX_INPUT_SIZE + 1];
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        cleanup_buffer(&input_data);
        cleanup_buffer(&compressed);
        cleanup_buffer(&decompressed);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        BUFFER_APPEND(input_data, (uint8_t)input_buffer[i]);
    }
    
    if (compress_rle(input_data.data, input_data.size, &compressed) != 0) {
        printf("Compression failed\n");
        cleanup_buffer(&input_data);
        cleanup_buffer(&compressed);
        cleanup_buffer(&decompressed);
        return EXIT_FAILURE;
    }
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        cleanup_buffer(&input_data);
        cleanup_buffer(&compressed);
        cleanup_buffer(&decompressed);
        return EXIT_FAILURE;
    }
    
    if (input_data.size != decompressed.size || 
        memcmp(input_data.data, decompressed.data, input_data.size) != 0) {
        printf("Verification failed\n");
        cleanup_buffer