//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
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

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return -1;
    if (SIZE_MAX - buf->size < len) return -1;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        if (new_capacity > MAX_OUTPUT_SIZE) return -1;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 0;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return -1;
    if (input_len > MAX_INPUT_SIZE) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        size_t count = 1;
        while (i + count < input_len && count < 255 && *(input + i + count) == current) {
            count++;
        }
        uint8_t count_byte = (uint8_t)count;
        if (buffer_append(output, &count_byte, 1) != 0) return -1;
        if (buffer_append(output, &current, 1) != 0) return -1;
        i += count;
    }
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return -1;
    if (input_len % 2 != 0) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t count = *(input + i);
        uint8_t value = *(input + i + 1);
        if (count == 0) return -1;
        if (SIZE_MAX - output->size < count) return -1;
        for (size_t j = 0; j < count; j++) {
            if (buffer_append(output, &value, 1) != 0) return -1;
        }
        i += 2;
    }
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter input data (up to %d bytes): ", MAX_INPUT_SIZE);
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_len < MAX_INPUT_SIZE) {
        char *endptr;
        long value = strtol(token, &endptr, 10);
        if (endptr == token || *endptr != '\0') {
            fprintf(stderr, "Invalid byte value: %s\n", token);
            return 1;
        }
        if (value < 0 || value > 255) {
            fprintf(stderr, "Invalid byte value: %s\n", token);
            return 1;
        }
        *(input_data + input_len) = (uint8_t)value;
        input_len++;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No valid input data\n");
        return 1;
    }
    
    Buffer compressed, decompressed;
    if (buffer_init(&compressed, 64) != 0) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    if (buffer_init(&decompressed, 64) != 0) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    
    if (compress_rle(input_data, input_len, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&comp