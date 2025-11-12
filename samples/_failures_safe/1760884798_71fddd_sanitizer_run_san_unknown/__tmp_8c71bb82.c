//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 4096
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} ByteBuffer;

void buffer_init(ByteBuffer *buf) {
    buf->data = malloc(BUFFER_SIZE);
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = BUFFER_SIZE;
}

void buffer_append(ByteBuffer *buf, uint8_t byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) abort();
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    buf->data[buf->size++] = byte;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

void run_length_encode(const uint8_t *input, size_t input_size, ByteBuffer *output) {
    if (input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        buffer_append(output, (uint8_t)count);
        buffer_append(output, current);
        i += count;
    }
}

int read_input(ByteBuffer *input) {
    uint8_t buffer[BUFFER_SIZE];
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            if (ferror(stdin)) return 0;
        }
        
        if (total_read + bytes_read > MAX_INPUT_SIZE) return 0;
        
        for (size_t i = 0; i < bytes_read; i++) {
            buffer_append(input, buffer[i]);
        }
        total_read += bytes_read;
    }
    
    return 1;
}

void write_output(const ByteBuffer *output) {
    if (fwrite(output->data, 1, output->size, stdout) != output->size) {
        abort();
    }
}

int main(void) {
    ByteBuffer input_data, output_data;
    
    buffer_init(&input_data);
    buffer_init(&output_data);
    
    if (!read_input(&input_data)) {
        buffer_free(&input_data);
        buffer_free(&output_data);
        return EXIT_FAILURE;
    }
    
    run_length_encode(input_data.data, input_data.size, &output_data);
    
    write_output(&output_data);
    
    buffer_free(&input_data);
    buffer_free(&output_data);
    
    return EXIT_SUCCESS;
}