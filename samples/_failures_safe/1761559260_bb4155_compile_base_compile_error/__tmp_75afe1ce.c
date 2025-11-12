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

void buffer_append(Buffer *buf, unsigned char value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        if (new_capacity <= buf->capacity) {
            return;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *input_end = input + input_size;
    unsigned char *output_ptr = output;
    unsigned char *output_end = output + output_capacity;
    
    while (input < input_end && output_ptr < output_end - 2) {
        unsigned char current = *input;
        size_t count = 1;
        
        const unsigned char *next = input + 1;
        while (next < input_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (count >= 3) {
            *output_ptr++ = current;
            *output_ptr++ = (unsigned char)count;
            input += count;
        } else {
            *output_ptr++ = current;
            input++;
        }
    }
    
    while (input < input_end && output_ptr < output_end) {
        *output_ptr++ = *input++;
    }
    
    return output_ptr - output;
}

size_t decompress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *input_end = input + input_size;
    unsigned char *output_ptr = output;
    unsigned char *output_end = output + output_capacity;
    
    while (input < input_end - 1 && output_ptr < output_end) {
        unsigned char value = *input++;
        
        if (input < input_end && *input >= 3) {
            unsigned char count = *input++;
            size_t remaining = output_end - output_ptr;
            if (count > remaining) {
                count = (unsigned char)remaining;
            }
            for (unsigned char i = 0; i < count && output_ptr < output_end; i++) {
                *output_ptr++ = value;
            }
        } else {
            if (output_ptr < output_end) {
                *output_ptr++ = value;
            }
        }
    }
    
    while (input < input_end && output_ptr < output_end) {
        *output_ptr++ = *input++;
    }
    
    return output_ptr - output;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char compressed[MAX_OUTPUT_SIZE];
    unsigned char decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input, input_len, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size < input_len) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
    } else {
        printf("No