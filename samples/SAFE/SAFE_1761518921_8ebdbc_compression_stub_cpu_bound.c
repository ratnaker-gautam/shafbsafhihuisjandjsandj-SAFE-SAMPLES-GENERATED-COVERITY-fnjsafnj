//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer* buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t len) {
    if (len == 0) return 1;
    if (buf->size > SIZE_MAX - len) return 0;
    
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity < CHUNK_SIZE) new_capacity = CHUNK_SIZE;
        
        uint8_t* new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void buffer_free(Buffer* buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int read_input(Buffer* input) {
    Buffer chunk;
    buffer_init(&chunk);
    
    uint8_t temp[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(temp, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&chunk, temp, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
        if (chunk.size > MAX_INPUT_SIZE) {
            buffer_free(&chunk);
            return 0;
        }
    }
    
    if (ferror(stdin)) {
        buffer_free(&chunk);
        return 0;
    }
    
    int result = buffer_append(input, chunk.data, chunk.size);
    buffer_free(&chunk);
    return result;
}

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t run[3] = {0xFF, current, (uint8_t)count};
            if (!buffer_append(output, run, 3)) return;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                uint8_t literal[1] = {current};
                if (!buffer_append(output, literal, 1)) return;
            }
            i += count;
        }
    }
}

int main(void) {
    Buffer input, output;
    buffer_init(&input);
    buffer_init(&output);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    simple_compress(input.data, input.size, &output);
    
    if (fwrite(output.data, 1, output.size, stdout) != output.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}