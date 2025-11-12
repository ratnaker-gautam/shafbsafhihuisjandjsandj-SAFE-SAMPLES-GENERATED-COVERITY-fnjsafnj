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

void buffer_append(Buffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity > MAX_OUTPUT_SIZE || new_capacity < buf->capacity) {
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
    *(buf->data + buf->size) = byte;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) {
            break;
        }
        
        *out_ptr++ = (unsigned char)count;
        *out_ptr++ = value;
        
        current += count;
    }
    
    return out_ptr - output;
}

int main(void) {
    char input_line[MAX_INPUT_SIZE + 1];
    Buffer input_buf;
    Buffer output_buf;
    
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&output_buf, MAX_OUTPUT_SIZE);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        buffer_append(&input_buf, (unsigned char)input_line[i]);
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_buf.data, output_buf.capacity);
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", compressed_size * 100.0 / input_buf.size);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", *(output_buf.data + i));
    }
    printf("\n");
    
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    
    return EXIT_SUCCESS;
}