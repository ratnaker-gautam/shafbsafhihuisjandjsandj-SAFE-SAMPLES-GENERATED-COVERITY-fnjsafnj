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

void process_input(void) {
    char stack_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (!fgets(stack_input, sizeof(stack_input), stdin)) {
        printf("Input error\n");
        return;
    }
    size_t input_len = strlen(stack_input);
    if (input_len > 0 && stack_input[input_len - 1] == '\n') {
        stack_input[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("Empty input\n");
        return;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        printf("Memory allocation failed\n");
        return;
    }
    simple_compress((uint8_t*)stack_input, input_len, &compressed);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i += 2) {
        if (i > 0) printf(" ");
        printf("%02X%02X", compressed.data[i], compressed.data[i+1]);
    }
    printf("\n");
    buffer_free(&compressed);
}

int main(void) {
    process_input();
    return 0;
}