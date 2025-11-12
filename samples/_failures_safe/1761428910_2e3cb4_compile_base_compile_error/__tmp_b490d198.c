//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (src == NULL || len == 0 || len > MAX_OUTPUT_SIZE) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity || new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src_end = src + len;
    while (src < src_end) {
        *dest = *src;
        dest++;
        src++;
    }
    buf->size += len;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || input_len > MAX_INPUT_SIZE || output_capacity < 2) {
        return 0;
    }
    const uint8_t *input_end = input + input_len;
    uint8_t *output_start = output;
    uint8_t *output_end = output + output_capacity;
    while (input < input_end) {
        uint8_t current = *input;
        size_t count = 1;
        const uint8_t *next = input + 1;
        while (next < input_end && *next == current && count < 255) {
            count++;
            next++;
        }
        if (output + 2 > output_end) {
            return 0;
        }
        *output = current;
        output++;
        *output = (uint8_t)count;
        output++;
        input += count;
    }
    return output - output_start;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || input_len > MAX_INPUT_SIZE || output_capacity == 0) {
        return 0;
    }
    const uint8_t *input_end = input + input_len;
    uint8_t *output_start = output;
    uint8_t *output_end = output + output_capacity;
    while (input < input_end) {
        if (input + 2 > input_end) {
            return 0;
        }
        uint8_t value = *input;
        input++;
        uint8_t count = *input;
        input++;
        if (output + count > output_end) {
            return 0;
        }
        uint8_t *dest = output;
        for (uint8_t i = 0; i < count; i++) {
            *dest = value;
            dest++;
        }
        output += count;
    }
    return output - output_start;
}

int main(void) {
    Buffer input_buf;
    Buffer output_buf;
    if (!buffer_init(&input_buf, 256) || !buffer_init(&output_buf, 512)) {
        fprintf(stderr, "Buffer initialization failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    char input_line[MAX_INPUT_SIZE + 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Input read failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    if (!buffer_append(&input_buf, (uint8_t*)input_line, input_len)) {
        fprintf(stderr, "Input buffer append failed\n");
        buffer_free(&input_buf);
        buffer_free(&output