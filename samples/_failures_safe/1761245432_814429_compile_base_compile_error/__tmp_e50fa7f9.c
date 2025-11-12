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
    if (buf == NULL || initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
        return -1;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return -1;
    }
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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0 || data_size > SIZE_MAX - buf->size) {
        return -1;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > SIZE_MAX / 2) {
            return -1;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return -1;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 0;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t stack_index = 0;
    uint8_t current_byte = input[0];
    uint8_t count = 1;
    
    for (size_t i = 1; i < input_size; i++) {
        if (input[i] == current_byte && count < UINT8_MAX) {
            count++;
        } else {
            if (stack_index + 2 > CHUNK_SIZE) {
                if (buffer_append(output, stack_buffer, stack_index) != 0) {
                    return;
                }
                stack_index = 0;
            }
            stack_buffer[stack_index++] = count;
            stack_buffer[stack_index++] = current_byte;
            current_byte = input[i];
            count = 1;
        }
    }
    if (stack_index + 2 <= CHUNK_SIZE) {
        stack_buffer[stack_index++] = count;
        stack_buffer[stack_index++] = current_byte;
    }
    if (stack_index > 0) {
        buffer_append(output, stack_buffer, stack_index);
    }
}

void simple_decompress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t stack_index = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t byte_val = input[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (stack_index >= CHUNK_SIZE) {
                if (buffer_append(output, stack_buffer, stack_index) != 0) {
                    return;
                }
                stack_index = 0;
            }
            stack_buffer[stack_index++] = byte_val;
        }
    }
    if (stack_index > 0) {
        buffer_append(output, stack_buffer, stack_index);
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    Buffer compressed;
    if (buffer_init(&compressed, CHUNK_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    simple_compress((uint8_t*)input_str, input_len, &compressed);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    if (input_len > 0) {
        double compression_ratio = (double)compressed.size / (double)