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
#define BUFFER_GROW(buffer, needed) \
    do { \
        size_t new_cap = (buffer).capacity == 0 ? 64 : (buffer).capacity * 2; \
        while (new_cap < (buffer).size + (needed)) new_cap *= 2; \
        uint8_t *new_data = realloc((buffer).data, new_cap); \
        if (!new_data) { \
            free((buffer).data); \
            exit(EXIT_FAILURE); \
        } \
        (buffer).data = new_data; \
        (buffer).capacity = new_cap; \
    } while(0)

#define BUFFER_APPEND(buffer, byte) \
    do { \
        if ((buffer).size >= (buffer).capacity) { \
            BUFFER_GROW(buffer, 1); \
        } \
        (buffer).data[(buffer).size++] = (byte); \
    } while(0)

static int buffer_init(Buffer *buf) {
    buf->data = malloc(64);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = 64;
    return 1;
}

static void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = buf->capacity = 0;
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
            
            if (count >= MIN_RUN_LENGTH && count <= MAX_RUN_LENGTH) {
                for (uint8_t j = 0; j < count; j++) {
                    BUFFER_APPEND(*output, value);
                }
                i += 3;
                continue;
            }
        }
        
        BUFFER_APPEND(*output, input[i]);
        i++;
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
    
    if (!buffer_init(buf)) return 0;
    
    for (size_t i = 0; i < total_read; i++) {
        BUFFER_APPEND(*buf, temp[i]);
    }
    
    return 1;
}

static void print_buffer(const Buffer *buf) {
    if (!buf || !buf->data) return;
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
}

int main(void) {
    Buffer input = BUFFER_INIT;
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (!read_input(&input)) {
        printf("Error reading input or input too large\n");
        return EXIT_FAILURE;
    }
    
    if (!buffer_init(&compressed)) {
        printf("Memory allocation failed\n");
        buffer_free(&input);
        return EXIT_FAILURE;
    }
    
    if (!buffer_init(&decompressed)) {
        printf("Memory allocation failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    size_t comp_size = compress_rle(input.data, input.size, &compressed);
    size