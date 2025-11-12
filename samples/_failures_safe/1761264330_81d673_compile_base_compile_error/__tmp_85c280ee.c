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
    if (!buf || !data || len == 0 || len > SIZE_MAX - buf->size) return 0;
    BUFFER_CHECK(buf, len);
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

static int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && run_length < MAX_RUN_LENGTH && 
               input[i + run_length] == current) {
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
    if (!input || !output || input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (input[i] == COMPRESSED_MARKER && i + 2 < input_len) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH || count > MAX_RUN_LENGTH) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            i += 3;
        } else {
            if (!buffer_append(output, &input[i], 1)) return 0;
            i++;
        }
    }
    
    return 1;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if (i < len - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    Buffer input = BUFFER_INIT;
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE + 2];
    if (!fgets(line, sizeof(line), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!buffer_append(&input, (uint8_t*)line, len)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Original (%zu bytes): ", input.size);
    print_hex(input.data, input.size);
    
    if (!compress_rle(input.data, input.size, &compressed)) {
        printf("Compression failed\n");
        free(input.data);
        return 1;
    }
    
    printf("Compressed (%zu bytes): ", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed\n");
        free(input.data);
        free(compressed.data);
        return 1;
    }