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

void run_length_encode(const uint8_t *input, size_t len, ByteBuffer *output) {
    if (len == 0) return;
    
    size_t i = 0;
    while (i < len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (!bytebuffer_append(output, current) || !bytebuffer_append(output, (uint8_t)count)) {
            return;
        }
        
        i = j;
    }
}

void run_length_decode(const uint8_t *input, size_t len, ByteBuffer *output) {
    if (len % 2 != 0) return;
    
    size_t i = 0;
    while (i < len) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (!bytebuffer_append(output, value)) return;
        }
        
        i += 2;
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
    
    ByteBuffer encoded;
    bytebuffer_init(&encoded);
    run_length_encode((uint8_t*)input_str, input_len, &encoded);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", encoded.size);
    printf("Compression ratio: %.2f%%\n", 
           (encoded.size * 100.0) / (input_len > 0 ? input_len : 1));
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < encoded.size; i++) {
        printf("%02X ", encoded.data[i]);
    }
    printf("\n");
    
    ByteBuffer decoded;
    bytebuffer_init(&decoded);
    run_length_decode(encoded.data, encoded.size, &decoded);
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < decoded.size; i++) {
        printf("%c", decoded.data[i]);
    }
    printf("\n");
    
    if (decoded.size == input_len && memcmp(decoded.data, input_str, input_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    bytebuffer_free(&encoded);
    bytebuffer_free(&decoded);
    return 0;
}