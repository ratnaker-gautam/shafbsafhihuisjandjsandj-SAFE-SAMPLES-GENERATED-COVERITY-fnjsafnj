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
    if (buf == NULL || initial_capacity == 0) return -1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return -1;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
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
    if (buf == NULL || src == NULL || len == 0) return -1;
    if (buf->size > SIZE_MAX - len) return -1;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        if (new_capacity > SIZE_MAX) return -1;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size += len;
    return 0;
}

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return -1;
    if (input_len > MAX_INPUT_SIZE) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        while (i + count < input_len && count < 255 && *(input + i + count) == current) {
            count++;
        }
        unsigned char count_byte = (unsigned char)count;
        if (buffer_append(output, &count_byte, 1) != 0) return -1;
        if (buffer_append(output, &current, 1) != 0) return -1;
        i += count;
    }
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return -1;
    if (input_len % 2 != 0) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char count = *(input + i);
        unsigned char value = *(input + i + 1);
        if (count == 0) return -1;
        for (size_t j = 0; j < count; j++) {
            if (buffer_append(output, &value, 1) != 0) return -1;
        }
        i += 2;
    }
    return 0;
}

int main() {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (buffer_init(&input_buf, MAX_INPUT_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    if (buffer_init(&compressed_buf, MAX_OUTPUT_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    if (buffer_init(&decompressed_buf, MAX_INPUT_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    char input_text[MAX_INPUT_SIZE + 1];
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
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
        fprintf(stderr, "Input is empty\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    if (buffer_append(&input_buf, (unsigned char*)input_text, input_len) != 0) {
        fprintf(stderr, "Failed to store input\n");
        buffer_free(&