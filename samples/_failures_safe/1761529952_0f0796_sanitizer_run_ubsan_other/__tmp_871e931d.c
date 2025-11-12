//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

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
    if (len > SIZE_MAX - buf->size) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_cap = buf->capacity ? buf->capacity * 2 : CHUNK_SIZE;
        if (new_cap < buf->size + len) new_cap = buf->size + len;
        if (new_cap > MAX_INPUT_SIZE) return 0;
        uint8_t* new_data = realloc(buf->data, new_cap);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
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
    if (!buffer_append(&chunk, (uint8_t*)"", 0)) return 0;
    
    uint8_t temp[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(temp, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, temp, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
        if (input->size > MAX_INPUT_SIZE) {
            buffer_free(&chunk);
            return 0;
        }
    }
    if (ferror(stdin)) {
        buffer_free(&chunk);
        return 0;
    }
    buffer_free(&chunk);
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input->size == 0) return;
    
    size_t run_length = 1;
    uint8_t current = input->data[0];
    
    for (size_t i = 1; i < input->size; i++) {
        if (input->data[i] == current && run_length < UINT8_MAX) {
            run_length++;
        } else {
            uint8_t header[2] = {current, (uint8_t)run_length};
            buffer_append(output, header, 2);
            current = input->data[i];
            run_length = 1;
        }
    }
    uint8_t header[2] = {current, (uint8_t)run_length};
    buffer_append(output, header, 2);
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
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    simple_compress(&input, &output);
    
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