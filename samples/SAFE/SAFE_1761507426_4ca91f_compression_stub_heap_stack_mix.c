//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.capacity = (initial_capacity > 0) ? initial_capacity : 1;
    buf.data = malloc(buf.capacity);
    buf.size = 0;
    if (buf.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return buf;
}

void append_to_buffer(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < buf->size + data_size) {
            if (new_capacity > SIZE_MAX / 2) {
                fprintf(stderr, "Buffer too large\n");
                exit(EXIT_FAILURE);
            }
            new_capacity *= 2;
        }
        
        uint8_t* new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
}

void free_buffer(Buffer* buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size && output_size < output_capacity - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[output_size++] = current;
            output[output_size++] = current;
            output[output_size++] = (uint8_t)count;
            i += count;
        } else {
            output[output_size++] = current;
            i++;
        }
    }
    
    return output_size;
}

int main(void) {
    Buffer input_buf = create_buffer(CHUNK_SIZE);
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    printf("Enter data to compress (max %d bytes):\n", MAX_INPUT_SIZE);
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (input_buf.size + bytes_read > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            free_buffer(&input_buf);
            return EXIT_FAILURE;
        }
        append_to_buffer(&input_buf, chunk, bytes_read);
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        free_buffer(&input_buf);
        return EXIT_FAILURE;
    }
    
    if (input_buf.size == 0) {
        printf("No input data provided\n");
        free_buffer(&input_buf);
        return EXIT_SUCCESS;
    }
    
    size_t max_output_size = input_buf.size * 2;
    if (max_output_size > SIZE_MAX - 1) {
        fprintf(stderr, "Output size calculation overflow\n");
        free_buffer(&input_buf);
        return EXIT_FAILURE;
    }
    
    uint8_t* output_data = malloc(max_output_size);
    if (output_data == NULL) {
        fprintf(stderr, "Output memory allocation failed\n");
        free_buffer(&input_buf);
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.size, output_data, max_output_size);
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100);
    
    if (compressed_size > 0) {
        size_t written = fwrite(output_data, 1, compressed_size, stdout);
        if (written != compressed_size) {
            fprintf(stderr, "Error writing output\n");
        }
    }
    
    free(output_data);
    free_buffer(&input_buf);
    
    return EXIT_SUCCESS;
}