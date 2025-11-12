//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return -1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return -1;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
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
    if (buf == NULL || data == NULL || data_size == 0) return -1;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) return -1;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src = data;
    size_t i = 0;
    while (i < data_size) {
        *dest = *src;
        dest++;
        src++;
        i++;
    }
    buf->size += data_size;
    return 0;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return -1;
    if (input_size > MAX_INPUT_SIZE) return -1;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        const uint8_t *run_start = current;
        uint8_t value = *current;
        
        while (current < end && *current == value && (current - run_start) < 255) {
            current++;
        }
        
        size_t run_length = current - run_start;
        
        if (run_length >= 3) {
            uint8_t encoded[2] = {value, (uint8_t)run_length};
            if (buffer_append(output, encoded, 2) != 0) return -1;
        } else {
            if (buffer_append(output, run_start, run_length) != 0) return -1;
        }
    }
    
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return -1;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        if (current + 1 < end && *(current + 1) >= 3) {
            uint8_t value = *current;
            uint8_t count = *(current + 1);
            
            if (output->size + count > MAX_OUTPUT_SIZE) return -1;
            
            uint8_t *dest = output->data + output->size;
            size_t i = 0;
            while (i < count) {
                *dest = value;
                dest++;
                i++;
            }
            output->size += count;
            current += 2;
        } else {
            if (buffer_append(output, current, 1) != 0) return -1;
            current++;
        }
    }
    
    return 0;
}

int main() {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (buffer_init(&input_buf, 256) != 0) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (buffer_init(&compressed_buf, 512) != 0) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    if (buffer_init(&decompressed_buf, 256) != 0) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    uint8_t temp_input[MAX_INPUT_SIZE + 1];
    if (fgets((char*)temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    size_t input_len = strlen((char*)temp_input