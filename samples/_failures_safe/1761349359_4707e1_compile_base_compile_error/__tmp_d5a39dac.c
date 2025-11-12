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
        while (new_capacity < buf->size + len) {
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
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_end = src + len;
    while (src < src_end) {
        *dest = *src;
        dest++;
        src++;
    }
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

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return 0;
    }
    if (input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    
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
            unsigned char run[3] = {0xFF, value, (unsigned char)count};
            buffer_append(output, run, 3);
        } else {
            buffer_append(output, current, count);
        }
        
        current += count;
    }
    
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return 0;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        if (current + 2 < end && *current == 0xFF) {
            unsigned char value = *(current + 1);
            unsigned char count = *(current + 2);
            
            if (count == 0) {
                return 0;
            }
            
            for (size_t i = 0; i < count; i++) {
                buffer_append(output, &value, 1);
            }
            
            current += 3;
        } else {
            buffer_append(output, current, 1);
            current++;
        }
    }
    
    return 1;
}

void print_hex(const unsigned char *data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    const unsigned char *end = data + len;
    const unsigned char *ptr = data;
    while (ptr < end) {
        printf("%02X ", *ptr);
        ptr++;
    }
    printf("\n");
}

int main(void) {
    unsigned char test_input[] = "AAAABBBCCCDDDEEEEFFFFGGGGHHHHIIIIJJJJ";
    size_t input_len = sizeof(test_input) - 1;
    
    Buffer compressed;
    buffer_init(&compressed, 64);
    
    Buffer decompressed;
    buffer_init(&decompressed, 64);
    
    printf("Original data (%zu bytes):\n", input_len);
    print_hex(test_input, input_len);
    
    if (compress_rle(test_input, input_len, &compressed)) {
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_hex(compressed.data, compressed.size);
        
        if (decompress_rle(compressed.data, compressed.size, &decompressed)) {
            printf("Decompressed data (%zu bytes):\n", decompressed.size);
            print_hex(decompressed.data, decompressed.size);
            
            if (decompressed.size == input_len