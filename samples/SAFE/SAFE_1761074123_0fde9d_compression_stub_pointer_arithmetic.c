//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_cap = buf->capacity * 2;
        if (new_cap < buf->capacity) exit(EXIT_FAILURE);
        unsigned char *new_data = realloc(buf->data, new_cap);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    *(buf->data + buf->size) = byte;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UCHAR_MAX && *(input + i + count) == current) {
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
        
        if (output->size > MAX_OUTPUT_SIZE) return -1;
    }
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0 || input_len % 2 != 0) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char value = *(input + i);
        unsigned char count = *(input + i + 1);
        
        if (count == 0) return -1;
        
        for (unsigned char j = 0; j < count; j++) {
            buffer_append(output, value);
            if (output->size > MAX_OUTPUT_SIZE) return -1;
        }
        
        i += 2;
    }
    return 0;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter input size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_len) != 1 || input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        unsigned int temp;
        if (scanf("%u", &temp) != 1 || temp > UCHAR_MAX) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        *(input + i) = (unsigned char)temp;
    }
    
    Buffer compressed;
    buffer_init(&compressed, input_len * 2);
    
    if (compress_rle(input, input_len, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%u ", *(compressed.data + i));
    }
    printf("\n");
    
    Buffer decompressed;
    buffer_init(&decompressed, input_len * 2);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%u ", *(decompressed.data + i));
    }
    printf("\n");
    
    if (decompressed.size != input_len || memcmp(input, decompressed.data, input_len) != 0) {
        printf("Data mismatch after decompression\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return EXIT_SUCCESS;
}