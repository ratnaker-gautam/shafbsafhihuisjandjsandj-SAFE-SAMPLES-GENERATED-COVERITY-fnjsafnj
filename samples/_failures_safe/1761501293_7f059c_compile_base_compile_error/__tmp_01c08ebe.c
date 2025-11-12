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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    if (buf->size > SIZE_MAX - data_size) return 0;
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) new_capacity = buf->size + data_size;
        if (new_capacity > SIZE_MAX) return 0;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    if (input_size > SIZE_MAX / 2) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        const uint8_t *next = current + 1;
        
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        uint8_t run[2] = {value, (uint8_t)count};
        if (!buffer_append(output, run, 2)) return 0;
        
        current = next;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    if (input_size % 2 != 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        
        if (output->size > SIZE_MAX - count) return 0;
        if (output->size + count > output->capacity) {
            size_t new_capacity = output->capacity * 2;
            if (new_capacity < output->size + count) new_capacity = output->size + count;
            if (new_capacity > SIZE_MAX) return 0;
            uint8_t *new_data = realloc(output->data, new_capacity);
            if (new_data == NULL) return 0;
            output->data = new_data;
            output->capacity = new_capacity;
        }
        
        uint8_t *dest = output->data + output->size;
        for (size_t i = 0; i < count; i++) {
            *dest = value;
            dest++;
        }
        output->size += count;
        
        current += 2;
    }
    
    return 1;
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer compressed, decompressed;
    if (!buffer_init(&compressed, input_len * 2)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (!buffer_init(&decompressed, input_len)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    if (!compress_rle((uint8_t*)input_str, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        buffer