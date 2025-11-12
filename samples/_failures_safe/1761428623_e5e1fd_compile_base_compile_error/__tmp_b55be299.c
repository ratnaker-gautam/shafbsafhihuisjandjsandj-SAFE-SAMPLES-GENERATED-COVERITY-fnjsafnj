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
    if (initial_capacity == 0) initial_capacity = 64;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

int buffer_append(ByteBuffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL) return 0;
    if (len == 0) return 1;
    if (buf->size > SIZE_MAX - len) return 0;
    
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_ptr = src;
    for (size_t i = 0; i < len; i++) {
        *dest++ = *src_ptr++;
    }
    buf->size = new_size;
    return 1;
}

int compress_rle(const unsigned char *input, size_t input_len, ByteBuffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && *(input + i + count) == current) {
            count++;
        }
        
        unsigned char run[2] = {current, (unsigned char)count};
        if (!buffer_append(output, run, 2)) return 0;
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, ByteBuffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len % 2 != 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char value = *(input + i);
        unsigned char count = *(input + i + 1);
        
        if (count == 0) return 0;
        
        for (unsigned char j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
        
        i += 2;
    }
    
    return 1;
}

void buffer_free(ByteBuffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int main(void) {
    ByteBuffer input_data;
    ByteBuffer compressed;
    ByteBuffer decompressed;
    
    buffer_init(&input_data, BUFFER_SIZE);
    buffer_init(&compressed, BUFFER_SIZE);
    buffer_init(&decompressed, BUFFER_SIZE);
    
    printf("Enter text to compress (max %d characters): ", BUFFER_SIZE - 1);
    
    unsigned char temp_buffer[BUFFER_SIZE];
    if (fgets((char*)temp_buffer, BUFFER_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)temp_buffer);
    if (input_len > 0 && temp_buffer[input_len - 1] == '\n') {
        temp_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    if (!buffer_append(&input_data, temp_buffer, input_len)) {
        printf("Error processing input\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input_data.data);
    printf("Original size: %zu bytes\n", input_data.size);
    
    if (!compress_rle(input_data.data, input_data.size, &compressed)) {
        printf("Compression failed\n");
        buffer_free(&input_data);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (input_data.size > 0) {
        float ratio = 100.0f * (float