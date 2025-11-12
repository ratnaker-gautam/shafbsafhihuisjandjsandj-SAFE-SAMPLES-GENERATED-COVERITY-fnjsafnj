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
    size_t pos = 0;
    while (pos < input_size) {
        uint8_t current = input[pos];
        size_t count = 1;
        while (pos + count < input_size && count < UINT8_MAX && input[pos + count] == current) {
            count++;
        }
        if (count >= 3) {
            uint8_t run_data[] = {0xFF, current, (uint8_t)count};
            if (buffer_append(output, run_data, 3) != 0) {
                return;
            }
            pos += count;
        } else {
            size_t literal_start = pos;
            while (pos < input_size && (pos - literal_start) < CHUNK_SIZE - 1) {
                if (pos + 3 < input_size && input[pos] == input[pos + 1] && input[pos] == input[pos + 2]) {
                    break;
                }
                pos++;
            }
            size_t literal_len = pos - literal_start;
            if (literal_len > 0) {
                if (literal_len > UINT8_MAX) {
                    literal_len = UINT8_MAX;
                }
                uint8_t literal_header[] = {0x00, (uint8_t)literal_len};
                if (buffer_append(output, literal_header, 2) != 0) {
                    return;
                }
                if (buffer_append(output, input + literal_start, literal_len) != 0) {
                    return;
                }
            }
        }
    }
}

void simple_decompress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    size_t pos = 0;
    while (pos < input_size) {
        if (input[pos] == 0xFF) {
            if (pos + 3 > input_size) {
                return;
            }
            uint8_t value = input[pos + 1];
            uint8_t count = input[pos + 2];
            for (uint8_t i = 0; i < count; i++) {
                if (buffer_append(output, &value, 1) != 0) {
                    return;
                }
            }
            pos += 3;
        } else if (input[pos] == 0x00) {
            if (pos + 2 > input_size) {
                return;
            }
            uint8_t length = input[pos + 1];
            if (pos + 2 + length > input_size) {
                return;
            }
            if (buffer_append(output, input + pos + 2, length) != 0) {
                return;
            }
            pos += 2 + length;
        } else {
            return;
        }
    }
}

int main() {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] =