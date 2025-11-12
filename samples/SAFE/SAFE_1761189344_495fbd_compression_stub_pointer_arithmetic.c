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
            free(buf->data);
            exit(EXIT_FAILURE);
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
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < 255) {
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

size_t read_input(unsigned char *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    
    size_t total_read = 0;
    unsigned char *ptr = buffer;
    unsigned char *end = buffer + max_size;
    
    while (ptr < end) {
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        *ptr++ = (unsigned char)c;
        total_read++;
    }
    
    return total_read;
}

void write_output(const unsigned char *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    const unsigned char *ptr = data;
    const unsigned char *end = data + size;
    
    while (ptr < end) {
        putchar(*ptr++);
    }
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char output[MAX_OUTPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    fflush(stdout);
    
    size_t input_size = read_input(input, MAX_INPUT_SIZE - 1);
    if (input_size == 0) {
        printf("No input received.\n");
        return EXIT_FAILURE;
    }
    
    *(input + input_size) = '\0';
    
    size_t compressed_size = compress_rle(input, input_size, output, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed or produced no output.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    write_output(output, compressed_size);
    printf("\n");
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * (double)compressed_size / (double)input_size) : 0.0);
    
    return EXIT_SUCCESS;
}