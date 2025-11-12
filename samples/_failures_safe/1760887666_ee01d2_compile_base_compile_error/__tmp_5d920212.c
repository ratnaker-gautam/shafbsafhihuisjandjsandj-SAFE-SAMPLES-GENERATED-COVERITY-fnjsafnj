//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0 || data_size > SIZE_MAX - buf->size) {
        return 0;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > SIZE_MAX / 2) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t pos = 0;
    while (pos < input_size) {
        uint8_t current = input[pos];
        size_t count = 1;
        while (pos + count < input_size && count < UINT8_MAX && input[pos + count] == current) {
            count++;
        }
        if (count >= 3) {
            if (output->size + 2 > output->capacity && !buffer_append(output, stack_buffer, 0)) {
                return;
            }
            uint8_t run_length = (count > UINT8_MAX) ? UINT8_MAX : count;
            uint8_t compressed_chunk[2] = {0xFF, run_length};
            if (!buffer_append(output, compressed_chunk, 2)) {
                return;
            }
            if (!buffer_append(output, &current, 1)) {
                return;
            }
            pos += run_length;
        } else {
            size_t literal_count = 0;
            while (pos + literal_count < input_size && literal_count < CHUNK_SIZE - 1 && 
                   (literal_count + 1 >= input_size || input[pos + literal_count] != input[pos + literal_count + 1] || 
                    (pos + literal_count + 2 < input_size && input[pos + literal_count] == input[pos + literal_count + 1] && input[pos + literal_count] == input[pos + literal_count + 2]))) {
                literal_count++;
            }
            if (literal_count > 0) {
                if (output->size + literal_count + 1 > output->capacity && !buffer_append(output, stack_buffer, 0)) {
                    return;
                }
                uint8_t literal_header = (literal_count > UINT8_MAX - 1) ? UINT8_MAX - 1 : literal_count;
                if (!buffer_append(output, &literal_header, 1)) {
                    return;
                }
                if (!buffer_append(output, input + pos, literal_count)) {
                    return;
                }
                pos += literal_count;
            }
        }
    }
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t input_size = 0;
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input_stack, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input_size = strlen((char*)input_stack);
    if (input_size > 0 && input_stack[input_size - 1] == '\n') {
        input_stack[--input_size] = '\0';
    }
    if (input_size == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, input_size / 2 + 64)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    simple_compress(input_stack, input_size, &compressed);
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    if (input_size > 0) {
        double ratio = (double)compressed.size /