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
            uint8_t marker = COMPRESSED_MARKER;
            if (!buffer_append(output, &marker, 1)) return 0;
            uint8_t length_byte = (uint8_t)run_length;
            if (!buffer_append(output, &length_byte, 1)) return 0;
            if (!buffer_append(output, &current, 1)) return 0;
            i += run_length;
        } else {
            size_t literal_count = 0;
            while (i + literal_count < input_len && literal_count < MAX_RUN_LENGTH && 
                   (literal_count + 1 >= input_len || 
                    input[i + literal_count] != input[i + literal_count + 1] ||
                    (i + literal_count + 2 < input_len && 
                     input[i + literal_count] == input[i + literal_count + 1] &&
                     input[i + literal_count] == input[i + literal_count + 2]))) {
                literal_count++;
                if (literal_count == MAX_RUN_LENGTH) break;
            }
            
            uint8_t count_byte = (uint8_t)literal_count;
            if (!buffer_append(output, &count_byte, 1)) return 0;
            if (!buffer_append(output, input + i, literal_count)) return 0;
            i += literal_count;
        }
    }
    
    return 1;
}

static int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t marker = input[i];
        
        if (marker == COMPRESSED_MARKER) {
            if (i + 2 >= input_len) return 0;
            
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (run_length < MIN_RUN_LENGTH || run_length > MAX_RUN_LENGTH) return 0;
            
            for (uint8_t j = 0; j < run_length; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            
            i += 3;
        } else {
            uint8_t literal_count = marker;
            if (literal_count == 0 || literal_count > MAX_RUN_LENGTH) return 0;
            if (i + literal_count >= input_len) return 0;
            
            if (!buffer_append(output, input + i + 1, literal_count)) return 0;
            i += 1 + literal_count;
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    size_t data_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && data_size < MAX_INPUT_SIZE) {
        original_data[data_size++] = (uint8_t)c;
    }
    
    if (data_size == 0) {
        printf("No input data provided.\n");