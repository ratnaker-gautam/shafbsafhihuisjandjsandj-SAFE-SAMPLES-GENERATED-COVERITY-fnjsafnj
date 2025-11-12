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

void bytebuffer_init(ByteBuffer *buf) {
    buf->capacity = 64;
    buf->data = malloc(buf->capacity);
    buf->size = 0;
}

void bytebuffer_free(ByteBuffer *buf) {
    if (buf->data) free(buf->data);
}

int bytebuffer_append(ByteBuffer *buf, uint8_t byte) {
    if (buf->size >= buf->capacity) {
        size_t new_cap = buf->capacity * 2;
        if (new_cap < buf->capacity) return 0;
        uint8_t *new_data = realloc(buf->data, new_cap);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    buf->data[buf->size++] = byte;
    return 1;
}

void compress_runlength(const uint8_t *input, size_t input_len, ByteBuffer *output) {
    if (input_len == 0) return;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (!bytebuffer_append(output, current)) return;
        if (!bytebuffer_append(output, (uint8_t)count)) return;
        
        i = j;
    }
}

void decompress_runlength(const uint8_t *input, size_t input_len, ByteBuffer *output) {
    if (input_len % 2 != 0) return;
    
    size_t idx = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        size_t j = 0;
        do {
            if (!bytebuffer_append(output, value)) return;
            j++;
        } while (j < count);
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Input error\n");
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
    bytebuffer_init(&compressed);
    bytebuffer_init(&decompressed);
    
    compress_runlength((uint8_t*)input_str, input_len, &compressed);
    
    if (compressed.size == 0) {
        printf("Compression failed\n");
        bytebuffer_free(&compressed);
        bytebuffer_free(&decompressed);
        return 1;
    }
    
    decompress_runlength(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input_str, input_len) != 0) {
        printf("Verification failed\n");
        bytebuffer_free(&compressed);
        bytebuffer_free(&decompressed);
        return 1;
    }
    
    printf("Original: %s\n", input_str);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed.size / input_len * 100.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        if (isprint(compressed.data[i])) {
            printf("%c", compressed.data[i]);
        } else {
            printf("[0x%02X]", compressed.data[i]);
        }
    }
    printf("\n");
    
    bytebuffer_free(&compressed);
    bytebuffer_free(&decompressed);
    return 0;
}