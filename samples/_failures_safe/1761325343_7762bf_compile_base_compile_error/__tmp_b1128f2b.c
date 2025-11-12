//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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
    memcpy(buf->data + buf->size, src, len);
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
        while (i + count < input_len && count < 255 && *(input + i + count) == current) {
            count++;
        }
        if (count >= 3 || current == 0xFF) {
            if (output->size + 2 > output->capacity) {
                if (!buffer_append(output, (unsigned char[]){0xFF, (unsigned char)count}, 2)) return 0;
            } else {
                *(output->data + output->size) = 0xFF;
                *(output->data + output->size + 1) = (unsigned char)count;
                output->size += 2;
            }
            if (output->size >= output->capacity) {
                if (!buffer_append(output, &current, 1)) return 0;
            } else {
                *(output->data + output->size) = current;
                output->size += 1;
            }
        } else {
            for (size_t j = 0; j < count; j++) {
                if (output->size >= output->capacity) {
                    if (!buffer_append(output, &current, 1)) return 0;
                } else {
                    *(output->data + output->size) = current;
                    output->size += 1;
                }
            }
        }
        i += count;
    }
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    if (input_len > MAX_OUTPUT_SIZE) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        if (*(input + i) == 0xFF && i + 1 < input_len) {
            unsigned char count = *(input + i + 1);
            if (i + 2 >= input_len) return 0;
            unsigned char value = *(input + i + 2);
            for (int j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            i += 3;
        } else {
            if (!buffer_append(output, input + i, 1)) return 0;
            i += 1;
        }
    }
    return 1;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, 256)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    if (!buffer_init(&compressed_buf, 512)) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    if (!buffer_init(&decompressed_buf, 256)) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    char input_line[MAX_INPUT_SIZE + 1];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf