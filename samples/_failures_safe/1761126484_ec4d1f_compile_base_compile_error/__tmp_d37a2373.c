//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
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

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (len == 0 || src == NULL) return;
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > MAX_OUTPUT_SIZE / 2) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
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

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    const uint8_t *input_end = input + input_len;
    uint8_t *output_ptr = output;
    uint8_t *output_end = output + output_capacity;
    
    while (input < input_end && output_ptr < output_end - 2) {
        uint8_t current = *input;
        size_t count = 1;
        
        const uint8_t *next = input + 1;
        while (next < input_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (count >= 3) {
            *output_ptr++ = current;
            *output_ptr++ = (uint8_t)count;
            input += count;
        } else {
            *output_ptr++ = current;
            input++;
        }
    }
    
    return output_ptr - output;
}

size_t read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    size_t total_read = 0;
    int c;
    while (total_read < max_size - 1 && (c = getchar()) != EOF) {
        buffer[total_read++] = (uint8_t)c;
    }
    buffer[total_read] = '\0';
    return total_read;
}

void decompress_rle(const uint8_t *compressed, size_t compressed_size, Buffer *decompressed) {
    if (compressed == NULL || decompressed == NULL) return;
    
    const uint8_t *comp_ptr = compressed;
    const uint8_t *comp_end = compressed + compressed_size;
    
    while (comp_ptr < comp_end) {
        if (comp_ptr + 1 < comp_end && *(comp_ptr + 1) >= 3) {
            uint8_t value = *comp_ptr;
            uint8_t count = *(comp_ptr + 1);
            
            for (uint8_t i = 0; i < count; i++) {
                if (decompressed->size < decompressed->capacity) {
                    decompressed->data[decompressed->size++] = value;
                }
            }
            comp_ptr += 2;
        } else {
            if (decompressed->size < decompressed->capacity) {
                decompressed->data[decompressed->size++] = *comp_ptr;
            }
            comp_ptr++;
        }
    }
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t output[MAX_OUTPUT_SIZE];
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE - 1);
    size_t input_len = read_input(input, MAX_INPUT_SIZE);
    
    if (input_len == 0) {
        printf("No input received.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    
    size_t compressed_size = compress_rle(input, input_len, output, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);