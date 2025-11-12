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
#define BUFFER_GROW_FACTOR 2
#define BUFFER_CHECK(b, needed) \
    if ((b)->size + (needed) > (b)->capacity) { \
        size_t new_cap = (b)->capacity ? (b)->capacity * BUFFER_GROW_FACTOR : 64; \
        while (new_cap < (b)->size + (needed)) new_cap *= BUFFER_GROW_FACTOR; \
        uint8_t *new_data = realloc((b)->data, new_cap); \
        if (!new_data) return 0; \
        (b)->data = new_data; \
        (b)->capacity = new_cap; \
    }

static int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0) return 0;
    BUFFER_CHECK(buf, len);
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

static int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && input[i + run_length] == current && run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= MIN_RUN_LENGTH) {
            uint8_t compressed[3] = {COMPRESSED_MARKER, current, (uint8_t)run_length};
            if (!buffer_append(output, compressed, 3)) return 0;
            i += run_length;
        } else {
            if (!buffer_append(output, &current, 1)) return 0;
            i++;
        }
    }
    return 1;
}

static int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        if (input[i] == COMPRESSED_MARKER && i + 2 < input_len) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            if (count < MIN_RUN_LENGTH) return 0;
            
            BUFFER_CHECK(output, count);
            for (uint8_t j = 0; j < count; j++) {
                output->data[output->size++] = value;
            }
            i += 3;
        } else {
            if (!buffer_append(output, &input[i], 1)) return 0;
            i++;
        }
    }
    return 1;
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
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        cleanup_buffer(&compressed);
        cleanup_buffer(&decompressed);
        return 1;
    }
    
    printf("Original: %s\n", input_str);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_rle((uint8_t*)input_str, input_len, &compressed)) {
        printf("Compression failed\n");
        cleanup_buffer(&compressed);
        cleanup_buffer(&decompressed);
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed.size / input_len * 100);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed\n");
        cleanup_buffer(&compressed);
        cleanup_buffer(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input_str, input_len) != 0) {
        printf("Verification failed\n");
        cleanup_buffer(&compressed);