//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) new_capacity *= 2;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        uint8_t run[2] = {value, (uint8_t)count};
        buffer_append(output, run, 2);
        
        current = next;
    }
    
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0) return 0;
    if (input_len % 2 != 0) return -1;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        
        if (count == 0) return -1;
        
        for (uint8_t i = 0; i < count; i++) {
            buffer_append(output, &value, 1);
        }
        
        current += 2;
    }
    
    return 0;
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
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Buffer compressed, decompressed;
    buffer_init(&compressed, 64);
    buffer_init(&decompressed, 64);
    
    if (compress_rle((uint8_t*)input_str, input_len, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input_str, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Decompression successful: ");
    fwrite(decompressed.data, 1, decompressed.size, stdout);
    printf("\n");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}