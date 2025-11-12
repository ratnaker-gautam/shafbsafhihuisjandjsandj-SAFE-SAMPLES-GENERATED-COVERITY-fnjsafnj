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
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t *out_len) {
    if (len == 0) {
        *out_len = 0;
        return;
    }
    size_t j = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (count > 2 || current == 0xFF) {
            output[j++] = 0xFF;
            output[j++] = current;
            output[j++] = (uint8_t)count;
        } else {
            for (size_t k = 0; k < count; k++) {
                output[j++] = current;
            }
        }
        i += count;
    }
    *out_len = j;
}

int main(void) {
    char input_stack[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_stack, sizeof(input_stack), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_stack);
    if (input_len > 0 && input_stack[input_len - 1] == '\n') {
        input_stack[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = input_len - i;
        if (chunk_len > CHUNK_SIZE) {
            chunk_len = CHUNK_SIZE;
        }
        uint8_t output_stack[CHUNK_SIZE * 2];
        size_t compressed_len;
        compress_chunk((uint8_t*)input_stack + i, chunk_len, output_stack, &compressed_len);
        if (!buffer_append(&compressed, output_stack, compressed_len)) {
            fprintf(stderr, "Compression buffer overflow\n");
            buffer_free(&compressed);
            return 1;
        }
    }
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
    }
    printf("\n");
    buffer_free(&compressed);
    return 0;
}