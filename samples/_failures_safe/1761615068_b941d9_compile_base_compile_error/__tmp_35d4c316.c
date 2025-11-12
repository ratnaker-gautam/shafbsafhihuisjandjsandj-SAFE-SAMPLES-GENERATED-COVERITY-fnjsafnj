//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
    
    buf->data = malloc(initial_capacity * sizeof(uint8_t));
    if (buf->data == NULL) return 0;
    
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL) return 0;
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return;
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 2) {
            if (out_idx + 3 > CHUNK_SIZE) break;
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            size_t literal_start = i;
            size_t literal_end = i;
            
            while (literal_end < input_size && literal_end - literal_start < UINT8_MAX) {
                if (literal_end + 2 < input_size && 
                    input[literal_end] == input[literal_end + 1] && 
                    input[literal_end] == input[literal_end + 2]) {
                    break;
                }
                literal_end++;
            }
            
            size_t literal_length = literal_end - literal_start;
            if (out_idx + literal_length + 1 > CHUNK_SIZE) {
                literal_length = CHUNK_SIZE - out_idx - 1;
                if (literal_length == 0) break;
            }
            
            output[out_idx++] = (uint8_t)literal_length;
            
            for (size_t j = 0; j < literal_length; j++) {
                output[out_idx++] = input[literal_start + j];
            }
            
            i = literal_start + literal_length;
        }
        
        if (out_idx >= CHUNK_SIZE - 3) break;
    }
    
    *output_size = out_idx;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
        uint8_t output_chunk[CHUNK_SIZE];
        size_t output_size;
        
        compress_chunk((uint8_t*)input + processed, chunk_size, output_chunk, &output_size);
        
        if (!buffer_append(&compressed, output_chunk, output_size)) {
            fprintf(stderr,