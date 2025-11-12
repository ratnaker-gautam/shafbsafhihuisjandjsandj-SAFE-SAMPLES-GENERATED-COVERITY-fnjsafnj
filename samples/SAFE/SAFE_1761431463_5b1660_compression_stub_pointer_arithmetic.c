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
        while (new_capacity < buf->size + len) {
            if (new_capacity > MAX_OUTPUT_SIZE / 2) {
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
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
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

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const uint8_t *input_end = input + input_size;
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

int main(void) {
    char input_str[MAX_INPUT_SIZE];
    Buffer input_buf, output_buf;
    
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&output_buf, MAX_OUTPUT_SIZE);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    buffer_append(&input_buf, (uint8_t*)input_str, input_len);
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, sizeof(compressed));
    
    if (compressed_size > 0) {
        buffer_append(&output_buf, compressed, compressed_size);
        
        printf("Original size: %zu bytes\n", input_buf.size);
        printf("Compressed size: %zu bytes\n", output_buf.size);
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)output_buf.size / input_buf.size) * 100);
        
        printf("Compressed data: ");
        for (size_t i = 0; i < output_buf.size && i < 50; i++) {
            printf("%02X ", output_buf.data[i]);
        }
        if (output_buf.size > 50) {
            printf("...");
        }
        printf("\n");
    } else {
        printf("Compression failed or produced no output.\n");
    }
    
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    
    return EXIT_SUCCESS;
}