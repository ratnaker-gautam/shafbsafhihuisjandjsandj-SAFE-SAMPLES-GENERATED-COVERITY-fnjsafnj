//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 0;
}

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 1) {
            unsigned char pair[2] = {current, (unsigned char)count};
            if (buffer_append(output, pair, 2) != 0) return -1;
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_len && (i + 1 >= input_len || *(input + i) != *(input + i + 1)) && (i - literal_start) < UCHAR_MAX) {
                i++;
            }
            size_t literal_len = i - literal_start;
            if (literal_len > 0) {
                unsigned char header[2] = {0, (unsigned char)literal_len};
                if (buffer_append(output, header, 2) != 0) return -1;
                if (buffer_append(output, input + literal_start, literal_len) != 0) return -1;
            }
        }
    }
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        if (i + 1 >= input_len) return -1;
        
        unsigned char marker = *(input + i);
        unsigned char count = *(input + i + 1);
        i += 2;
        
        if (marker == 0) {
            if (i + count > input_len) return -1;
            if (buffer_append(output, input + i, count) != 0) return -1;
            i += count;
        } else {
            if (count == 0) return -1;
            for (size_t j = 0; j < count; j++) {
                if (buffer_append(output, &marker, 1) != 0) return -1;
            }
        }
    }
    return 0;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_len;
    
    printf("Enter input size (max %d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_len) != 1 || input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data: ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        int byte;
        if (scanf("%d", &byte) != 1 || byte < 0 || byte > 255) {
            printf("Invalid byte value\n");
            return 1;
        }
        *(input + i) = (unsigned char)byte;
    }
    
    Buffer compressed;
    if (buffer_init(&compressed, input_len * 2) != 0) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (compress_rle(input, input_len, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size