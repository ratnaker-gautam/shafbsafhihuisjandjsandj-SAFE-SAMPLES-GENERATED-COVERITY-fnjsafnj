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
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0 || src == NULL) {
        return;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > MAX_OUTPUT_SIZE) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            free(buf->data);
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    if (buf->size + len <= buf->capacity) {
        memcpy(buf->data + buf->size, src, len);
        buf->size += len;
    }
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        unsigned char run[2] = {value, (unsigned char)count};
        buffer_append(output, run, 2);
        
        current = next;
    }
    
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return -1;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    
    while (current < end - 1) {
        unsigned char value = *current;
        unsigned char count = *(current + 1);
        
        if (count == 0) {
            return -1;
        }
        
        for (unsigned char i = 0; i < count; i++) {
            if (output->size >= output->capacity) {
                size_t new_capacity = output->capacity * 2;
                if (new_capacity > MAX_OUTPUT_SIZE) {
                    new_capacity = MAX_OUTPUT_SIZE;
                }
                unsigned char *new_data = realloc(output->data, new_capacity);
                if (new_data == NULL) {
                    return -1;
                }
                output->data = new_data;
                output->capacity = new_capacity;
            }
            *(output->data + output->size) = value;
            output->size++;
        }
        
        current += 2;
    }
    
    return 0;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size++] = (unsigned char)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    buffer_init(&compressed, 64);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100.0);
    
    Buffer decompressed;
    buffer_init(&decompressed, input_size * 2);
    
    if (decompress_rle(compressed.data