//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} ByteBuffer;

void buffer_init(ByteBuffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(ByteBuffer *buf, const unsigned char *src, size_t len) {
    if (len == 0) return;
    if (buf->size > SIZE_MAX - len) abort();
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) abort();
            new_capacity *= 2;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size = new_size;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

void compress_rle(const unsigned char *input, size_t input_size, ByteBuffer *output) {
    if (input == NULL || output == NULL) return;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        unsigned char compressed[2] = {value, (unsigned char)count};
        buffer_append(output, compressed, 2);
        
        current = next;
    }
}

void decompress_rle(const unsigned char *input, size_t input_size, ByteBuffer *output) {
    if (input == NULL || output == NULL || input_size % 2 != 0) return;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    
    while (current < end) {
        unsigned char value = *current;
        unsigned char count = *(current + 1);
        
        for (unsigned char i = 0; i < count; i++) {
            buffer_append(output, &value, 1);
        }
        
        current += 2;
    }
}

int main(void) {
    ByteBuffer input, compressed, decompressed;
    
    buffer_init(&input, BUFFER_SIZE);
    buffer_init(&compressed, BUFFER_SIZE);
    buffer_init(&decompressed, BUFFER_SIZE);
    
    printf("Enter text to compress (max %d characters): ", BUFFER_SIZE - 1);
    
    char temp_buffer[BUFFER_SIZE];
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_buffer);
    if (input_len > 0 && temp_buffer[input_len - 1] == '\n') {
        temp_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    buffer_append(&input, (unsigned char*)temp_buffer, input_len);
    
    compress_rle(input.data, input.size, &compressed);
    decompress_rle(compressed.data, compressed.size, &decompressed);
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (input.size == decompressed.size && 
        memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
    }
    
    buffer_free(&input);
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}