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

void buffer_append(ByteBuffer *buf, const unsigned char *src, size_t count) {
    if (count == 0) return;
    if (buf->size > SIZE_MAX - count) abort();
    size_t new_size = buf->size + count;
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
    memcpy(buf->data + buf->size, src, count);
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
    
    size_t pos = 0;
    while (pos < input_size) {
        unsigned char current = *(input + pos);
        size_t count = 1;
        
        while (pos + count < input_size && count < 255 && *(input + pos + count) == current) {
            count++;
        }
        
        unsigned char encoded[2] = { (unsigned char)count, current };
        buffer_append(output, encoded, 2);
        
        pos += count;
    }
}

void decompress_rle(const unsigned char *input, size_t input_size, ByteBuffer *output) {
    if (input == NULL || output == NULL) return;
    
    size_t pos = 0;
    while (pos + 1 < input_size) {
        unsigned char count = *(input + pos);
        unsigned char value = *(input + pos + 1);
        
        if (count == 0) {
            pos += 2;
            continue;
        }
        
        for (unsigned char i = 0; i < count; i++) {
            buffer_append(output, &value, 1);
        }
        
        pos += 2;
    }
}

int main(void) {
    ByteBuffer input_data, compressed, decompressed;
    buffer_init(&input_data, BUFFER_SIZE);
    buffer_init(&compressed, BUFFER_SIZE);
    buffer_init(&decompressed, BUFFER_SIZE);
    
    unsigned char test_data[] = "AAAABBBCCDAA";
    size_t test_size = sizeof(test_data) - 1;
    
    buffer_append(&input_data, test_data, test_size);
    
    compress_rle(input_data.data, input_data.size, &compressed);
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    
    if (input_data.size != decompressed.size) {
        printf("Error: Size mismatch\n");
        return 1;
    }
    
    if (memcmp(input_data.data, decompressed.data, input_data.size) != 0) {
        printf("Error: Data mismatch\n");
        return 1;
    }
    
    printf("Original: ");
    for (size_t i = 0; i < input_data.size; i++) {
        printf("%c", *(input_data.data + i));
    }
    printf("\n");
    
    printf("Compressed: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", *(compressed.data + i));
    }
    printf("\n");
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", *(decompressed.data + i));
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / (double)input_data.size) * 100.0);
    
    buffer_free(&input_data);
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}