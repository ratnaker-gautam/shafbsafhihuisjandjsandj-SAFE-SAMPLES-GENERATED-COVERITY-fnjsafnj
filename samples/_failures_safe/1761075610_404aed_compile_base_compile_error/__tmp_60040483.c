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
#define BUFFER_CHECK(b, need) \
    if ((b)->size + (need) > (b)->capacity) { \
        size_t new_cap = (b)->capacity ? (b)->capacity * BUFFER_GROW_FACTOR : 64; \
        while (new_cap < (b)->size + (need)) new_cap *= BUFFER_GROW_FACTOR; \
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
    size_t pos = 0;
    
    while (pos < input_len) {
        uint8_t current = input[pos];
        size_t run_length = 1;
        
        while (pos + run_length < input_len && run_length < MAX_RUN_LENGTH && 
               input[pos + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= MIN_RUN_LENGTH) {
            uint8_t compressed[3] = {COMPRESSED_MARKER, current, (uint8_t)run_length};
            if (!buffer_append(output, compressed, 3)) return 0;
            pos += run_length;
        } else {
            if (!buffer_append(output, &current, 1)) return 0;
            pos++;
        }
    }
    
    return 1;
}

static int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    
    output->size = 0;
    size_t pos = 0;
    
    while (pos < input_len) {
        if (input[pos] == COMPRESSED_MARKER && pos + 2 < input_len) {
            uint8_t value = input[pos + 1];
            uint8_t count = input[pos + 2];
            
            if (count < MIN_RUN_LENGTH || count > MAX_RUN_LENGTH) return 0;
            
            for (uint8_t i = 0; i < count; i++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            pos += 3;
        } else {
            if (!buffer_append(output, &input[pos], 1)) return 0;
            pos++;
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
    uint8_t original[] = {0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x43, 0x44, 0x45, 0x45, 0x46};
    size_t original_len = sizeof(original);
    
    printf("Original data (%zu bytes): ", original_len);
    print_hex(original, original_len);
    
    Buffer compressed = BUFFER_INIT;
    if (!compress_rle(original, original_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    Buffer decompressed = BUFFER_INIT;
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == original_len && 
        memcmp(decompressed.data, original, original_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");