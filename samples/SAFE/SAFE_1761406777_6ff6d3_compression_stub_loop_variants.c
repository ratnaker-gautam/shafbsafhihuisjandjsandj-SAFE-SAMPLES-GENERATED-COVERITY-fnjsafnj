//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} ByteBuffer;

void buffer_init(ByteBuffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 16;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(ByteBuffer *buf, uint8_t byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) abort();
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    buf->data[buf->size++] = byte;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

void compress_run_length(const uint8_t *input, size_t input_size, ByteBuffer *output) {
    if (input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        buffer_append(output, count);
        buffer_append(output, current);
        
        i = j;
    }
}

void decompress_run_length(const uint8_t *input, size_t input_size, ByteBuffer *output) {
    if (input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        if (i + 1 >= input_size) break;
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t c = 0; c < count; c++) {
            buffer_append(output, value);
        }
        
        i += 2;
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
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
    
    ByteBuffer compressed, decompressed;
    buffer_init(&compressed, 16);
    buffer_init(&decompressed, input_len);
    
    compress_run_length((uint8_t*)input_str, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100.0);
    
    printf("Compressed data (hex): ");
    for (size_t idx = 0; idx < compressed.size; idx++) {
        printf("%02X ", compressed.data[idx]);
    }
    printf("\n");
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size == input_len && 
        memcmp(decompressed.data, input_str, input_len) == 0) {
        printf("Decompression successful: %s\n", (char*)decompressed.data);
    } else {
        printf("Decompression failed\n");
    }
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}