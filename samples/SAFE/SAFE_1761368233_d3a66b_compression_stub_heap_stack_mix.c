//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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
    if (initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (!buf->data) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len == 0) {
        return 1;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                return 0;
            }
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input_len == 0) {
        return;
    }
    uint8_t current = input[0];
    size_t count = 1;
    for (size_t i = 1; i < input_len; i++) {
        if (input[i] == current && count < UINT8_MAX) {
            count++;
        } else {
            uint8_t chunk[2] = {current, (uint8_t)count};
            if (!buffer_append(output, chunk, 2)) {
                return;
            }
            current = input[i];
            count = 1;
        }
    }
    uint8_t chunk[2] = {current, (uint8_t)count};
    buffer_append(output, chunk, 2);
}

void process_input(const char *input_str) {
    size_t input_len = strlen(input_str);
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return;
    }
    Buffer input_buf;
    if (!buffer_init(&input_buf, input_len)) {
        printf("Memory allocation failed\n");
        return;
    }
    if (!buffer_append(&input_buf, (const uint8_t*)input_str, input_len)) {
        printf("Buffer append failed\n");
        buffer_free(&input_buf);
        return;
    }
    Buffer output_buf;
    if (!buffer_init(&output_buf, CHUNK_SIZE)) {
        printf("Memory allocation failed\n");
        buffer_free(&input_buf);
        return;
    }
    simple_compress(input_buf.data, input_buf.size, &output_buf);
    printf("Original size: %zu\n", input_buf.size);
    printf("Compressed size: %zu\n", output_buf.size);
    printf("Compression ratio: %.2f\n", output_buf.size / (float)input_buf.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < output_buf.size; i += 2) {
        if (i + 1 < output_buf.size) {
            printf("(%c,%u) ", output_buf.data[i], output_buf.data[i+1]);
        }
    }
    printf("\n");
    buffer_free(&input_buf);
    buffer_free(&output_buf);
}

int main() {
    char input[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    process_input(input);
    return 0;
}