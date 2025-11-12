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
    if (input_len > MAX_INPUT_SIZE) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 2 || current == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, current);
            buffer_append(output, (unsigned char)count);
        } else {
            for (size_t j = 0; j < count; j++) {
                buffer_append(output, current);
            }
        }
        
        i += count;
        
        if (output->size > MAX_OUTPUT_SIZE) return -1;
    }
    
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0) return -1;
    
    size_t i = 0;
    while (i < input_len) {
        if (*(input + i) == 0xFF) {
            if (i + 2 >= input_len) return -1;
            
            unsigned char value = *(input + i + 1);
            unsigned char count = *(input + i + 2);
            
            if (count == 0) return -1;
            
            for (int j = 0; j < count; j++) {
                buffer_append(output, value);
                if (output->size > MAX_OUTPUT_SIZE) return -1;
            }
            
            i += 3;
        } else {
            buffer_append(output, *(input + i));
            i++;
            if (output->size > MAX_OUTPUT_SIZE) return -1;
        }
    }
    
    return 0;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter input data (up to %d bytes, end with EOF): ", MAX_INPUT_SIZE);
    
    int ch;
    while ((ch = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        if (ch < 0 || ch > 255) continue;
        *(input_data + input_len) = (unsigned char)ch;
        input_len++;
    }
    
    if (input_len == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    buffer_init(&compressed, 64);
    
    if (compress_rle(input_data, input_len, &compressed) != 0) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu\n", input_len);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100);
    
    Buffer decompressed;
    buffer_init(&decompressed, 64);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input_data, input_len) != 0) {
        printf("Verification failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Verification successful.\n");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return EXIT_SUCCESS;
}