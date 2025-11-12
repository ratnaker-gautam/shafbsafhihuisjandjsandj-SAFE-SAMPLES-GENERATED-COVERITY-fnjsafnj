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
    if (buf == NULL || initial_capacity == 0) return -1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return -1;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf == NULL || data == NULL || len == 0) return -1;
    if (buf->size > SIZE_MAX - len) return -1;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return -1;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 0;
}

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return;
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (count >= 3) {
            stack_buffer[0] = current;
            stack_buffer[1] = (uint8_t)count;
            if (buffer_append(output, stack_buffer, 2) != 0) return;
            i += count;
        } else {
            size_t literal_count = 0;
            while (i + literal_count < input_len && literal_count < CHUNK_SIZE - 1) {
                if (i + literal_count + 2 < input_len && 
                    input[i + literal_count] == input[i + literal_count + 1] &&
                    input[i + literal_count] == input[i + literal_count + 2]) {
                    break;
                }
                literal_count++;
            }
            if (literal_count > 0) {
                stack_buffer[0] = (uint8_t)literal_count;
                memcpy(stack_buffer + 1, input + i, literal_count);
                if (buffer_append(output, stack_buffer, literal_count + 1) != 0) return;
                i += literal_count;
            } else {
                stack_buffer[0] = input[i];
                stack_buffer[1] = 1;
                if (buffer_append(output, stack_buffer, 2) != 0) return;
                i++;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    Buffer compressed;
    if (buffer_init(&compressed, input_len) != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    simple_compress((uint8_t*)input, input_len, &compressed);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
    buffer_free(&compressed);
    return 0;
}