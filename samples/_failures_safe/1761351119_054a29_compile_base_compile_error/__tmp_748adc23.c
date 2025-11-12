//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
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

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size += len;
    return 1;
}

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    if (input_len > MAX_INPUT_SIZE) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        while (i + count < input_len && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        if (count >= 3) {
            unsigned char run[2] = {current, (unsigned char)count};
            if (!buffer_append(output, run, 2)) return 0;
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_len && (i - literal_start < UCHAR_MAX) && 
                   (i + 1 >= input_len || *(input + i) != *(input + i + 1) || 
                    (i + 2 < input_len && *(input + i) == *(input + i + 1) && *(input + i) != *(input + i + 2)))) {
                i++;
            }
            size_t literal_len = i - literal_start;
            if (literal_len > 0) {
                unsigned char *literal_data = malloc(literal_len + 1);
                if (literal_data == NULL) return 0;
                *literal_data = (unsigned char)literal_len;
                for (size_t j = 0; j < literal_len; j++) {
                    *(literal_data + 1 + j) = *(input + literal_start + j);
                }
                if (!buffer_append(output, literal_data, literal_len + 1)) {
                    free(literal_data);
                    return 0;
                }
                free(literal_data);
            }
        }
    }
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char marker = *(input + i);
        if (marker == 0) {
            i++;
            continue;
        }
        if (i + 1 >= input_len) return 0;
        
        if (marker > 128) {
            unsigned char value = *(input + i + 1);
            size_t count = marker - 128;
            for (size_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            i += 2;
        } else {
            size_t literal_len = marker;
            if (i + 1 + literal_len > input_len) return 0;
            if (!buffer_append(output, input + i + 1, literal_len)) return 0;
            i += 1 + literal_len;
        }
    }
    return 1;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Error: Failed to initialize input buffer\n");
        return 1;
    }
    if (!buffer_init(&compressed_buf, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Error: Failed to initialize compressed buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    if (!buffer_init(&decompressed_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr