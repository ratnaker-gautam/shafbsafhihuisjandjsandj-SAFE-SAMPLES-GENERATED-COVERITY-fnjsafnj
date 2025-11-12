//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t capacity) {
    if (capacity == 0 || capacity > MAX_OUTPUT_SIZE) return 0;
    buf->data = malloc(capacity);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (!src || len == 0) return 0;
    if (buf->size + len > buf->capacity) return 0;
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *end = src + len;
    while (src < end) {
        *dest = *src;
        dest++;
        src++;
    }
    buf->size += len;
    return 1;
}

int compress_run_length(const unsigned char *input, size_t input_len, Buffer *output) {
    if (!input || input_len == 0 || !output) return 0;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        if (count >= 3) {
            if (output->size + 2 > output->capacity) return 0;
            unsigned char *out_ptr = output->data + output->size;
            *out_ptr = value;
            *(out_ptr + 1) = (unsigned char)count;
            output->size += 2;
            current = next;
        } else {
            if (output->size + count > output->capacity) return 0;
            unsigned char *out_ptr = output->data + output->size;
            const unsigned char *copy_end = current + count;
            while (current < copy_end) {
                *out_ptr = *current;
                out_ptr++;
                current++;
            }
            output->size += count;
        }
    }
    
    return 1;
}

void print_hex(const unsigned char *data, size_t len) {
    if (!data || len == 0) return;
    
    const unsigned char *end = data + len;
    const unsigned char *current = data;
    
    while (current < end) {
        printf("%02x ", *current);
        current++;
    }
    printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
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
    
    Buffer compressed;
    if (!buffer_init(&compressed, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (!compress_run_length((unsigned char*)input_str, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);
    
    printf("Original data (hex): ");
    print_hex((unsigned char*)input_str, input_len);
    
    printf("Compressed data (hex): ");
    print_hex(compressed.data, compressed.size);
    
    buffer_free(&compressed);
    return 0;
}