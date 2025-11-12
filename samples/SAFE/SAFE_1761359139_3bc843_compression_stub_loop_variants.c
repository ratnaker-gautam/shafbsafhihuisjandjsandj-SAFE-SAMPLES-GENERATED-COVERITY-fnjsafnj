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
    free(buf->data);
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

void run_length_encode(const uint8_t *input, size_t input_len, ByteBuffer *output) {
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

void run_length_decode(const uint8_t *input, size_t input_len, ByteBuffer *output) {
    if (input_len % 2 != 0) return;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t value = input[i++];
        uint8_t count = input[i++];
        
        for (uint8_t j = 0; j < count; j++) {
            if (!bytebuffer_append(output, value)) return;
        }
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    ByteBuffer compressed, decompressed;
    bytebuffer_init(&compressed);
    bytebuffer_init(&decompressed);
    
    run_length_encode((uint8_t*)input_str, input_len, &compressed);
    
    printf("Original: %s\n", input_str);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        run_length_decode(compressed.data, compressed.size, &decompressed);
        
        if (decompressed.size == input_len && 
            memcmp(decompressed.data, input_str, input_len) == 0) {
            printf("Decompression successful\n");
        } else {
            printf("Decompression failed\n");
        }
        
        printf("Compressed data: ");
        for (size_t i = 0; i < compressed.size; i += 2) {
            if (isprint(compressed.data[i])) {
                printf("%c", compressed.data[i]);
            } else {
                printf("[0x%02X]", compressed.data[i]);
            }
            printf("x%u ", compressed.data[i + 1]);
        }
        printf("\n");
    }
    
    bytebuffer_free(&compressed);
    bytebuffer_free(&decompressed);
    return 0;
}