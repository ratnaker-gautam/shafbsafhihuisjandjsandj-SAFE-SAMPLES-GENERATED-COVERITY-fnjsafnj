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
#define BUFFER_GROW(buffer, min_capacity) \
    do { \
        size_t new_capacity = (buffer).capacity * 2; \
        if (new_capacity < min_capacity) new_capacity = min_capacity; \
        if (new_capacity < 16) new_capacity = 16; \
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
            BUFFER_GROW((buffer), (buffer).size + 1); \
        } \
        (buffer).data[(buffer).size++] = (byte); \
    } while(0)

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size == 0) return -1;
    
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

static int read_input(Buffer *buffer) {
    char input_line[MAX_INPUT_SIZE + 2];
    
    if (!fgets(input_line, sizeof(input_line), stdin)) {
        return -1;
    }
    
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_SIZE) {
        return -1;
    }
    
    buffer->size = 0;
    for (size_t i = 0; i < len; i++) {
        BUFFER_APPEND(*buffer, (uint8_t)input_line[i]);
    }
    
    return 0;
}

int main(void) {
    Buffer input = BUFFER_INIT;
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    int ret = EXIT_SUCCESS;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (read_input(&input) != 0) {
        printf("Error reading input\n");
        ret = EXIT_FAILURE;
        goto cleanup;
    }
    
    if (input.size == 0) {
        printf("No input provided\n");
        ret = EXIT_FAILURE;
        goto cleanup;
    }
    
    if (compress_rle(input.data, input.size, &compressed) != 0) {
        printf("Compression failed\n");
        ret = EXIT_FAILURE;
        goto cleanup;
    }
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input.size) * 100.0);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        ret = EXIT_FAILURE;
        goto cleanup;
    }
    
    if (decompressed.size != input.size || 
        memcmp