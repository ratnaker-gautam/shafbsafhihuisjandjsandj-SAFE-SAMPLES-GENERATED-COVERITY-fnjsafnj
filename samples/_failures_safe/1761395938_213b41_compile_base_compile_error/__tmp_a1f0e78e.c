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
        if (buf->size + len > new_capacity) {
            return;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
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
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        if (count >= 3) {
            unsigned char run[2] = {value, (unsigned char)count};
            buffer_append(output, run, 2);
        } else {
            buffer_append(output, current, count);
        }
        
        current += count;
    }
    
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    
    while (current < end) {
        if (current + 1 < end && *(current + 1) >= 3) {
            unsigned char value = *current;
            size_t count = *(current + 1);
            
            if (count > 100) {
                return -1;
            }
            
            for (size_t i = 0; i < count; i++) {
                buffer_append(output, &value, 1);
            }
            
            current += 2;
        } else {
            buffer_append(output, current, 1);
            current++;
        }
    }
    
    return 0;
}

void print_buffer_hex(const unsigned char *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    unsigned char test_input[] = {0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x43, 0x43, 0x44, 0x45};
    size_t input_size = sizeof(test_input);
    
    printf("Original data (%zu bytes):\n", input_size);
    print_buffer_hex(test_input, input_size);
    
    Buffer compressed;
    buffer_init(&compressed, 64);
    
    if (compress_rle(test_input, input_size, &compressed) == 0) {
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_buffer_hex(compressed.data, compressed.size);
        
        Buffer decompressed;
        buffer_init(&decompressed, 64);
        
        if (decompress_rle(compressed.data, compressed.size, &decompressed) == 0) {
            printf("Decompressed data (%zu bytes):\n