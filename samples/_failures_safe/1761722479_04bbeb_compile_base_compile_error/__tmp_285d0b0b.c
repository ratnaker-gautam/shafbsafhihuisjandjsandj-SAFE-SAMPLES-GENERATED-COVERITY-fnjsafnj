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
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src = data;
    size_t i = 0;
    while (i < data_size) {
        *(dest + i) = *(src + i);
        i++;
    }
    buf->size += data_size;
    return 1;
}

void compress_run_length(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < UINT8_MAX) {
            count++;
            next++;
        }
        
        uint8_t run_data[2] = {value, (uint8_t)count};
        if (!buffer_append(output, run_data, 2)) {
            return;
        }
        
        current = next;
    }
}

int decompress_run_length(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    if (input_size % 2 != 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end - 1) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        
        if (output->size + count > output->capacity) {
            size_t new_capacity = output->capacity * 2;
            if (new_capacity < output->size + count) {
                new_capacity = output->size + count;
            }
            if (new_capacity > MAX_OUTPUT_SIZE) return 0;
            
            uint8_t *new_data = realloc(output->data, new_capacity);
            if (new_data == NULL) return 0;
            output->data = new_data;
            output->capacity = new_capacity;
        }
        
        uint8_t *dest = output->data + output->size;
        size_t i = 0;
        while (i < count) {
            *(dest + i) = value;
            i++;
        }
        output->size += count;
        
        current += 2;
    }
    
    return 1;
}

int main() {
    printf("Run-Length Encoding Compression Demo\n");
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char input_text[MAX_INPUT_SIZE];
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer compressed, decompressed;
    if (!buffer_init(&compressed, 64) || !buffer_init(&decompressed, MAX_INPUT_SIZE)) {
        printf("Memory allocation failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    compress_run_length((uint8_t*)input_text, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed