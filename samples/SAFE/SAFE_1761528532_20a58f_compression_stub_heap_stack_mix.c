//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
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
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void compress_chunk(const Chunk *input, Chunk *output) {
    if (input == NULL || output == NULL) return;
    output->size = 0;
    for (size_t i = 0; i < input->size; i++) {
        if (output->size < CHUNK_SIZE) {
            output->data[output->size] = input->data[i] ^ 0x55;
            output->size++;
        }
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    Buffer input_buffer;
    if (!buffer_init(&input_buffer, input_len)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    if (!buffer_append(&input_buffer, (uint8_t*)input_str, input_len)) {
        fprintf(stderr, "Buffer append failed\n");
        buffer_free(&input_buffer);
        return 1;
    }
    Buffer output_buffer;
    if (!buffer_init(&output_buffer, input_len)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input_buffer);
        return 1;
    }
    size_t processed = 0;
    while (processed < input_buffer.size) {
        Chunk input_chunk;
        size_t chunk_size = input_buffer.size - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        memcpy(input_chunk.data, input_buffer.data + processed, chunk_size);
        input_chunk.size = chunk_size;
        Chunk output_chunk;
        compress_chunk(&input_chunk, &output_chunk);
        if (!buffer_append(&output_buffer, output_chunk.data, output_chunk.size)) {
            fprintf(stderr, "Output buffer append failed\n");
            buffer_free(&input_buffer);
            buffer_free(&output_buffer);
            return 1;
        }
        processed += chunk_size;
    }
    printf("Original size: %zu bytes\n", input_buffer.size);
    printf("Compressed size: %zu bytes\n", output_buffer.size);
    printf("Compression ratio: %.2f%%\n", 
           (output_buffer.size * 100.0) / input_buffer.size);
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output_buffer.size; i++) {
        printf("%02x", output_buffer.data[i]);
    }
    printf("\n");
    buffer_free(&input_buffer);
    buffer_free(&output_buffer);
    return 0;
}