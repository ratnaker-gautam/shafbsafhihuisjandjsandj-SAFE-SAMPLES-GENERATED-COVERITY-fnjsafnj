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

void buffer_append(ByteBuffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity <= buf->capacity) abort();
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = byte;
    buf->size++;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const unsigned char *input, size_t input_size, ByteBuffer *output) {
    if (input == NULL || output == NULL) return -1;
    if (input_size == 0) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 1) {
            buffer_append(output, current);
            buffer_append(output, (unsigned char)count);
            i += count;
        } else {
            buffer_append(output, current);
            buffer_append(output, 1);
            i++;
        }
    }
    
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_size, ByteBuffer *output) {
    if (input == NULL || output == NULL) return -1;
    if (input_size == 0) return 0;
    if (input_size % 2 != 0) return -1;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char value = *(input + i);
        unsigned char count = *(input + i + 1);
        
        if (count == 0) return -1;
        
        for (unsigned char j = 0; j < count; j++) {
            buffer_append(output, value);
        }
        
        i += 2;
    }
    
    return 0;
}

void print_hex(const unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", *(data + i));
    }
    printf("\n");
}

int main(void) {
    unsigned char test_data[] = {0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x44, 0x45, 0x45, 0x45};
    size_t test_size = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("Original data (%zu bytes): ", test_size);
    print_hex(test_data, test_size);
    
    ByteBuffer compressed;
    buffer_init(&compressed, 16);
    
    if (compress_rle(test_data, test_size, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    ByteBuffer decompressed;
    buffer_init(&decompressed, 16);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == test_size && memcmp(decompressed.data, test_data, test_size) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}