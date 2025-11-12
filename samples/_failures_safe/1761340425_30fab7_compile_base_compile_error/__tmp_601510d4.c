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

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size > SIZE_MAX - len) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        if (new_capacity > SIZE_MAX / 2) return 0;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

void compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return;
    
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
        
        unsigned char compressed[2] = {value, (unsigned char)count};
        if (!buffer_append(output, compressed, 2)) return;
        
        current = next;
    }
}

void decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len % 2 != 0) return;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        unsigned char value = *current;
        unsigned char count = *(current + 1);
        
        for (size_t i = 0; i < count; i++) {
            if (!buffer_append(output, &value, 1)) return;
        }
        
        current += 2;
    }
}

int read_input(unsigned char *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter input data (max %zu bytes): ", max_size);
    fflush(stdout);
    
    size_t total_read = 0;
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        *(buffer + total_read) = (unsigned char)c;
        total_read++;
    }
    
    return total_read;
}

void print_buffer(const unsigned char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    
    const unsigned char *current = buffer;
    const unsigned char *end = buffer + size;
    
    while (current < end) {
        if (*current >= 32 && *current <= 126) {
            putchar(*current);
        } else {
            printf("\\x%02X", *current);
        }
        current++;
    }
    printf("\n");
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    int input_len = read_input(input, MAX_INPUT_SIZE);
    
    if (input_len <= 0) {
        printf("No input provided or input too large.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_len * 2)) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    compress_rle(input, input_len, &compressed);
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer(compressed.data, compressed.size);
    
    Buffer decompressed;
    if (!buffer_init(&decompressed, input_len)) {
        printf("Memory allocation failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer(decompressed.data, decompressed.size);
    
    if (decompressed.size == (size_t)input_len && memcmp(decompressed.data, input, input_len) ==