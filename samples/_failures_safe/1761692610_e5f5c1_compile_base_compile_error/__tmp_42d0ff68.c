//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define MAX_OUTPUT_SIZE 2097152

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
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
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) {
            return 0;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
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
    buf->size = new_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        uint8_t run[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run, 2)) {
            return 0;
        }
        i += count;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        if (count == 0) {
            return 0;
        }
        for (uint8_t j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) {
                return 0;
            }
        }
    }
    return 1;
}

int read_input(Buffer *buf) {
    size_t total_read = 0;
    size_t chunk_size = 4096;
    if (!buffer_init(buf, chunk_size)) {
        return 0;
    }
    while (total_read < MAX_INPUT_SIZE) {
        if (buf->size == buf->capacity) {
            size_t new_capacity = buf->capacity * 2;
            if (new_capacity < buf->capacity || new_capacity > MAX_INPUT_SIZE) {
                return 0;
            }
            uint8_t *new_data = realloc(buf->data, new_capacity);
            if (new_data == NULL) {
                return 0;
            }
            buf->data = new_data;
            buf->capacity = new_capacity;
        }
        size_t to_read = buf->capacity - buf->size;
        if (to_read > chunk_size) {
            to_read = chunk_size;
        }
        size_t bytes_read = fread(buf->data + buf->size, 1, to_read, stdin);
        if (bytes_read == 0) {
            break;
        }
        buf->size += bytes_read;
        total_read += bytes_read;
    }
    if (ferror(stdin)) {
        return 0;
    }
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    int mode = 0;
    char mode_str[16];
    if (fgets(mode_str, sizeof(mode_str), stdin) == NULL) {
        fprintf(stderr, "Error reading mode\n");
        return 1;
    }
    if (strcmp(mode_str, "compress\n") == 0) {
        mode = 1;
    } else if (strcmp(mode_str, "decompress\n") == 0) {
        mode = 2;
    } else {
        fprintf(stderr, "Invalid mode. Use 'compress' or 'decompress'\n");
        return 1;
    }
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading