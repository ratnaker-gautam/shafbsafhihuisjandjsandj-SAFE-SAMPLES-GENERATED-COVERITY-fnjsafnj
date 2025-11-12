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

static int buffer_init(Buffer *buf) {
    buf->data = malloc(16);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = 16;
    return 1;
}

static void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

static size_t compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    
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
    
    return output->size;
}

static size_t decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH) {
                BUFFER_APPEND(*output, COMPRESSED_MARKER);
                i++;
                continue;
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
    
    return output->size;
}

static int read_input(Buffer *buf) {
    uint8_t temp[MAX_INPUT_SIZE];
    size_t total_read = 0;
    
    while (total_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        if (total_read < MAX_INPUT_SIZE) {
            temp[total_read++] = (uint8_t)c;
        }
    }
    
    if (total_read == 0) return 0;
    
    for (size_t i = 0; i < total_read; i++) {
        BUFFER_APPEND(*buf, temp[i]);
    }
    
    return 1;
}

int main(void) {
    Buffer input = BUFFER_INIT;
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    int result = EXIT_FAILURE;
    
    if (!buffer_init(&input) || !buffer_init(&compressed) || !buffer_init(&decompressed)) {
        fprintf(stderr, "Memory allocation failed\n");
        goto cleanup;
    }
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (!read_input(&input)) {
        fprintf(stderr, "No input received\n");
        goto cleanup;
    }
    
    printf("Original size: %zu bytes\n", input.size);
    
    if (!compress_rle(input.data, input.size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        goto cleanup;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0 && input.size > 0)