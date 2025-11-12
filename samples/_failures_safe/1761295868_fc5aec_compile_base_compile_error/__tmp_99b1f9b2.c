//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_INPUT_SIZE) {
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
    if (len > MAX_INPUT_SIZE - buf->size) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return 0;
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

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t run[2] = {0xFF, current};
            if (!buffer_append(output, run, 2)) {
                return 0;
            }
            uint8_t count_byte = (uint8_t)count;
            if (!buffer_append(output, &count_byte, 1)) {
                return 0;
            }
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                uint8_t literal = input[i + j];
                if (literal == 0xFF) {
                    uint8_t escape[2] = {0xFF, 0x00};
                    if (!buffer_append(output, escape, 2)) {
                        return 0;
                    }
                } else {
                    if (!buffer_append(output, &literal, 1)) {
                        return 0;
                    }
                }
            }
            i += count;
        }
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t i = 0;
    while (i < input_len) {
        if (input[i] == 0xFF) {
            if (i + 1 >= input_len) {
                return 0;
            }
            if (input[i + 1] == 0x00) {
                uint8_t literal = 0xFF;
                if (!buffer_append(output, &literal, 1)) {
                    return 0;
                }
                i += 2;
            } else {
                if (i + 2 >= input_len) {
                    return 0;
                }
                uint8_t value = input[i + 1];
                uint8_t count = input[i + 2];
                if (count == 0) {
                    return 0;
                }
                for (uint8_t j = 0; j < count; j++) {
                    if (!buffer_append(output, &value, 1)) {
                        return 0;
                    }
                }
                i += 3;
            }
        } else {
            if (!buffer_append(output, &input[i], 1)) {
                return 0;
            }
            i++;
        }
    }
    return 1;
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!buffer_init(&input_data, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input_data);
        return 1;
    }
    if (!buffer_init(&decompressed, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        return 1;
    }

    printf