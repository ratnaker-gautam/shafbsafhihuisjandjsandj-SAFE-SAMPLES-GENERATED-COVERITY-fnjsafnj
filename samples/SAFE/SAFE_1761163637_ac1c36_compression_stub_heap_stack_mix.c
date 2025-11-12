//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer buffer_create(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0 && initial_capacity <= MAX_INPUT_SIZE) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int buffer_resize(Buffer* buf, size_t new_capacity) {
    if (new_capacity == 0 || new_capacity > MAX_INPUT_SIZE) {
        return 0;
    }
    uint8_t* new_data = realloc(buf->data, new_capacity);
    if (!new_data) {
        return 0;
    }
    buf->data = new_data;
    buf->capacity = new_capacity;
    if (buf->size > buf->capacity) {
        buf->size = buf->capacity;
    }
    return 1;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (!buf || !data || data_size == 0) {
        return 0;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return 0;
        }
        if (!buffer_resize(buf, new_capacity)) {
            return 0;
        }
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void buffer_free(Buffer* buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size && output_size < output_capacity - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        if (output_size + 2 > output_capacity) {
            break;
        }
        output[output_size++] = current;
        output[output_size++] = (uint8_t)count;
        i += count;
    }
    return output_size;
}

int main(void) {
    uint8_t stack_buffer[CHUNK_SIZE];
    Buffer input_buf = buffer_create(CHUNK_SIZE);
    Buffer output_buf = buffer_create(CHUNK_SIZE);
    
    if (!input_buf.data || !output_buf.data) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    size_t bytes_read;
    while ((bytes_read = fread(stack_buffer, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&input_buf, stack_buffer, bytes_read)) {
            fprintf(stderr, "Input buffer overflow\n");
            buffer_free(&input_buf);
            buffer_free(&output_buf);
            return 1;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Input read error\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    if (input_buf.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    if (!buffer_resize(&output_buf, input_buf.size * 2)) {
        fprintf(stderr, "Output buffer allocation failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.size, output_buf.data, output_buf.capacity);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    size_t bytes_written = fwrite(output_buf.data, 1, compressed_size, stdout);
    if (bytes_written != compressed_size) {
        fprintf(stderr, "Output write error\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    return 0;
}