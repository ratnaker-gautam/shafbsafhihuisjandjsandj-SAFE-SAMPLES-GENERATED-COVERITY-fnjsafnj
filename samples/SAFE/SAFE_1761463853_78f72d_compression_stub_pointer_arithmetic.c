//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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

void buffer_append(Buffer *buf, const unsigned char *src, size_t count) {
    if (count == 0 || src == NULL) return;
    
    if (buf->size > SIZE_MAX - count) {
        exit(EXIT_FAILURE);
    }
    
    size_t new_size = buf->size + count;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                exit(EXIT_FAILURE);
            }
            new_capacity *= 2;
            if (new_capacity > MAX_OUTPUT_SIZE) {
                exit(EXIT_FAILURE);
            }
        }
        
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_ptr = src;
    for (size_t i = 0; i < count; i++) {
        *dest++ = *src_ptr++;
    }
    buf->size = new_size;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t simple_compress(const unsigned char *input, size_t input_size, 
                      unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (in_ptr < in_end && out_ptr + 2 <= out_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        
        const unsigned char *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (count >= 3) {
            *out_ptr++ = current;
            *out_ptr++ = (unsigned char)count;
            in_ptr += count;
        } else {
            *out_ptr++ = current;
            in_ptr++;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    char input_line[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    buffer_init(&compressed, input_len * 2);
    
    unsigned char *temp_output = malloc(input_len * 2);
    if (temp_output == NULL) {
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = simple_compress((unsigned char*)input_line, input_len, 
                                           temp_output, input_len * 2);
    
    if (compressed_size > 0) {
        buffer_append(&compressed, temp_output, compressed_size);
    }
    
    free(temp_output);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    if (compressed.size > 0) {
        printf("Compressed data (hex): ");
        for (size_t i = 0; i < compressed.size; i++) {
            printf("%02x ", compressed.data[i]);
        }
        printf("\n");
    }
    
    buffer_free(&compressed);
    return EXIT_SUCCESS;
}