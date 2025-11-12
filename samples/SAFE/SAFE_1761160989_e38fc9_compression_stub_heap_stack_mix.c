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

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0 && initial_capacity <= SIZE_MAX / 2) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int resize_buffer(Buffer *buf, size_t new_capacity) {
    if (new_capacity == 0 || new_capacity > SIZE_MAX / 2) {
        return 0;
    }
    uint8_t *new_data = realloc(buf->data, new_capacity);
    if (!new_data) {
        return 0;
    }
    buf->data = new_data;
    buf->capacity = new_capacity;
    if (buf->size > buf->capacity) {
        buf->size = buf->capacity;
    }
    return 1;
}

void free_buffer(Buffer *buf) {
    if (buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int append_to_buffer(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!buf || !data || data_size == 0 || data_size > SIZE_MAX - buf->size) {
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
        if (!resize_buffer(buf, new_capacity)) {
            return 0;
        }
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) {
        return;
    }
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        uint8_t chunk[2] = {current, (uint8_t)count};
        if (!append_to_buffer(output, chunk, 2)) {
            break;
        }
        i += count;
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (!buffer || max_size == 0) {
        return 0;
    }
    size_t total_read = 0;
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        buffer[total_read++] = (uint8_t)c;
    }
    return total_read;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    int input_size = read_input(input_stack, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }

    Buffer compressed = create_buffer(CHUNK_SIZE);
    if (!compressed.data) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    simple_compress(input_stack, input_size, &compressed);

    if (compressed.size > 0) {
        for (size_t i = 0; i < compressed.size; i++) {
            printf("%02X", compressed.data[i]);
        }
        printf("\n");
    } else {
        fprintf(stderr, "Error: Compression produced no output\n");
    }

    free_buffer(&compressed);
    return 0;
}