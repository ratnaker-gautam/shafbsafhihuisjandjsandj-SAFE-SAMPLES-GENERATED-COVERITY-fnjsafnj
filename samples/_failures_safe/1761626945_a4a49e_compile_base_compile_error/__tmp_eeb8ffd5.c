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
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output->size + 3 > output->capacity) {
                size_t new_capacity = output->capacity * 2;
                unsigned char *new_data = realloc(output->data, new_capacity);
                if (new_data == NULL) return 0;
                output->data = new_data;
                output->capacity = new_capacity;
            }
            *(output->data + output->size) = 0xFF;
            *(output->data + output->size + 1) = current;
            *(output->data + output->size + 2) = (unsigned char)count;
            output->size += 3;
        } else {
            if (output->size + count > output->capacity) {
                size_t new_capacity = output->capacity * 2;
                unsigned char *new_data = realloc(output->data, new_capacity);
                if (new_data == NULL) return 0;
                output->data = new_data;
                output->capacity = new_capacity;
            }
            for (size_t j = 0; j < count; j++) {
                *(output->data + output->size + j) = current;
            }
            output->size += count;
        }
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        if (*(input + i) == 0xFF && i + 2 < input_len) {
            unsigned char value = *(input + i + 1);
            unsigned char count = *(input + i + 2);
            
            if (output->size + count > output->capacity) {
                size_t new_capacity = output->capacity * 2;
                unsigned char *new_data = realloc(output->data, new_capacity);
                if (new_data == NULL) return 0;
                output->data = new_data;
                output->capacity = new_capacity;
            }
            
            for (unsigned char j = 0; j < count; j++) {
                *(output->data + output->size + j) = value;
            }
            output->size += count;
            i += 3;
        } else {
            if (output->size + 1 > output->capacity) {
                size_t new_capacity = output->capacity * 2;
                unsigned char *new_data = realloc(output->data, new_capacity);
                if (new_data == NULL) return 0;
                output->data = new_data;
                output->capacity = new_capacity;
            }
            *(output->data + output->size) = *(input + i);
            output->size += 1;
            i += 1;
        }
    }
    
    return 1;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    Buffer compressed, decompressed;
    
    if (!buffer_init(&compressed, 256)) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    
    if (!buffer_init(&decompressed, 256)) {
        fprintf(stderr, "Failed