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
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len > SIZE_MAX - buf->size) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity || new_capacity > SIZE_MAX / 2) {
            return 0;
        }
        if (buf->size + len > new_capacity) {
            new_capacity = buf->size + len;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
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

void process_input_stack(const uint8_t *input, size_t input_len) {
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return;
    }
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_len = input_len - processed;
        if (chunk_len > CHUNK_SIZE) {
            chunk_len = CHUNK_SIZE;
        }
        memcpy(stack_buffer, input + processed, chunk_len);
        for (size_t i = 0; i < chunk_len; i++) {
            stack_buffer[i] ^= 0x55;
        }
        printf("Processed chunk: ");
        for (size_t i = 0; i < chunk_len; i++) {
            printf("%02X ", stack_buffer[i]);
        }
        printf("\n");
        processed += chunk_len;
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, 64)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    simple_compress((uint8_t*)input_str, input_len, &compressed);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (compressed.size * 100.0) / input_len);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i += 2) {
        if (i + 1 < compressed.size) {
            printf("(%c:%u) ", compressed.data[i], compressed.data[i + 1]);
        }
    }
    printf("\n");
    printf("Processing input with stack buffers:\n");
    process_input_stack((uint8_t*)input_str, input_len);
    buffer_free(&compressed);
    return 0;
}