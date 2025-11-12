//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t count) {
    if (src == NULL || count == 0 || count > MAX_INPUT_SIZE) {
        return;
    }
    if (buf->size + count > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + count) {
            new_capacity = buf->size + count;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, count);
    buf->size += count;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    if (input_size > MAX_INPUT_SIZE || output_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }

    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;

    while (in_ptr < in_end && out_ptr + 2 <= out_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        const uint8_t *scan = in_ptr + 1;
        while (scan < in_end && *scan == current && count < 255) {
            count++;
            scan++;
        }

        *out_ptr++ = current;
        *out_ptr++ = (uint8_t)count;
        in_ptr += count;
    }

    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    if (input_size > MAX_INPUT_SIZE || output_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    if (input_size % 2 != 0) {
        return 0;
    }

    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;

    while (in_ptr + 1 < in_end && out_ptr < out_end) {
        uint8_t value = *in_ptr++;
        uint8_t count = *in_ptr++;
        
        if (out_ptr + count > out_end) {
            return 0;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }

    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, 64);
    buffer_init(&compressed_buf, 64);
    buffer_init(&decompressed_buf, 64);

    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    uint8_t temp_buf[MAX_INPUT_SIZE + 1];
    if (fgets((char*)temp_buf, sizeof(temp_buf), stdin) == NULL) {
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }

    size_t input_len = strlen((char*)temp_buf);
    if (input_len > 0 && temp_buf[input_len - 1] == '\n') {
        temp_buf[--input_len] = '\0';
    }

    if (input_len == 0) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }

    buffer_append(&input_buf, temp_buf, input_len);

    uint8_t compressed[MAX_OUTPUT_SIZE