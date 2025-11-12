//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size = new_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        size_t count = 1;
        while (i + count < input_len && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        if (count > 1) {
            uint8_t run[2] = {current, (uint8_t)count};
            if (!buffer_append(output, run, 2)) return 0;
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_len && (i - literal_start < UINT8_MAX) && (i + 1 >= input_len || *(input + i) != *(input + i + 1) || (i + 2 < input_len && *(input + i + 1) != *(input + i + 2)))) {
                i++;
            }
            size_t literal_len = i - literal_start;
            if (literal_len > 0) {
                uint8_t header = (uint8_t)literal_len;
                if (!buffer_append(output, &header, 1)) return 0;
                if (!buffer_append(output, input + literal_start, literal_len)) return 0;
            }
        }
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    size_t i = 0;
    while (i < input_len) {
        uint8_t header = *(input + i);
        i++;
        if (i >= input_len) return 0;
        if (header == 0) {
            uint8_t count = *(input + i);
            i++;
            if (i >= input_len) return 0;
            uint8_t value = *(input + i);
            i++;
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
        } else {
            size_t literal_len = header;
            if (i + literal_len > input_len) return 0;
            if (!buffer_append(output, input + i, literal_len)) return 0;
            i += literal_len;
        }
    }
    return 1;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    if (!buffer_init(&input_buf, BUFFER_SIZE) || !buffer_init(&compressed_buf, BUFFER_SIZE) || !buffer_init(&decompressed_buf, BUFFER_SIZE)) {
        fprintf(stderr, "Buffer initialization failed\n");
        return 1;
    }

    printf("Enter text to compress (max %d characters): ", BUFFER_SIZE - 1);
    char input_text[BUFFER_SIZE];
    if (fgets(input_text, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Input reading failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }

    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("