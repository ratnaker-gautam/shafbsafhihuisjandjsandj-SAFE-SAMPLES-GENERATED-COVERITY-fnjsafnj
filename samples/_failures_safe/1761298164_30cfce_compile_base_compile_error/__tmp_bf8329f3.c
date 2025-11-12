//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, unsigned char value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity == 0 ? 16 : buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    buf->data[buf->size++] = value;
    return 1;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_run_length_recursive(const unsigned char *input, size_t pos, size_t len, Buffer *output) {
    if (pos >= len) return 1;
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < UCHAR_MAX) {
        count++;
    }
    
    if (count > 1) {
        if (!buffer_append(output, current)) return 0;
        if (!buffer_append(output, (unsigned char)count)) return 0;
    } else {
        if (!buffer_append(output, current)) return 0;
    }
    
    return compress_run_length_recursive(input, pos + count, len, output);
}

int decompress_run_length_recursive(const unsigned char *input, size_t pos, size_t len, Buffer *output) {
    if (pos >= len) return 1;
    
    unsigned char current = input[pos];
    
    if (pos + 1 < len && input[pos + 1] > 1) {
        size_t count = input[pos + 1];
        for (size_t i = 0; i < count; i++) {
            if (!buffer_append(output, current)) return 0;
        }
        return decompress_run_length_recursive(input, pos + 2, len, output);
    } else {
        if (!buffer_append(output, current)) return 0;
        return decompress_run_length_recursive(input, pos + 1, len, output);
    }
}

int main(void) {
    char input_line[MAX_INPUT_SIZE + 1];
    Buffer input_data, compressed, decompressed;
    
    buffer_init(&input_data);
    buffer_init(&compressed);
    buffer_init(&decompressed);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_line, sizeof(input_line), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!buffer_append(&input_data, input_line[i])) {
            printf("Memory allocation failed\n");
            buffer_free(&input_data);
            buffer_free(&compressed);
            buffer_free(&decompressed);
            return 1;
        }
    }
    
    if (!compress_run_length_recursive(input_data.data, 0, input_data.size, &compressed)) {
        printf("Compression failed\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (!decompress_run_length_recursive(compressed.data, 0, compressed.size, &decompressed)) {
        printf("Decompression failed\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_data.size || memcmp(decompressed.data, input_data.data, input_data.size) != 0) {
        printf("Verification failed\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_data.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_data.size) * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        if (compressed.data[i] >= 32 && compressed.data[i] <= 126) {
            printf("%c", compressed.data[i]);
        } else {
            printf("[0x%02X]", compressed.data[i]);
        }