//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.data = NULL;
    buf.size = 0;
    buf.capacity = 0;
    
    if (initial_capacity > 0 && initial_capacity <= SIZE_MAX / 2) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / 2) return 0;
        
        uint8_t* new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void free_buffer(Buffer* buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!input || !output || input_size == 0) return 0;
    
    for (size_t i = 0; i < input_size; i++) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < 255) {
            count++;
            i++;
        }
        
        uint8_t chunk[2] = {current, (uint8_t)count};
        if (!buffer_append(output, chunk, 2)) return 0;
    }
    
    return 1;
}

int simple_decompress(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!input || !output || input_size == 0 || input_size % 2 != 0) return 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (count == 0) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
    }
    
    return 1;
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(CHUNK_SIZE);
    if (!compressed.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (!simple_compress((uint8_t*)input_str, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        free_buffer(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    Buffer decompressed = create_buffer(CHUNK_SIZE);
    if (!decompressed.data) {
        fprintf(stderr, "Memory allocation failed\n");
        free_buffer(&compressed);
        return 1;
    }
    
    if (!simple_decompress(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input_str, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    printf("Decompression successful: verified