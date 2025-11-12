//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

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
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (current < end && out_ptr < out_end - 2) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        *out_ptr++ = value;
        *out_ptr++ = (uint8_t)count;
        
        current += count;
    }
    
    return out_ptr - output;
}

int main(void) {
    char input_buf[MAX_INPUT_SIZE + 1];
    Buffer output;
    buffer_init(&output, 128);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buf, sizeof(input_buf), stdin) == NULL) {
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buf);
    if (input_len > 0 && input_buf[input_len - 1] == '\n') {
        input_buf[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle((uint8_t*)input_buf, input_len, compressed, sizeof(compressed));
    
    if (compressed_size > 0) {
        buffer_append(&output, compressed, compressed_size);
        
        printf("Original size: %zu bytes\n", input_len);
        printf("Compressed size: %zu bytes\n", compressed_size);
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
        
        printf("Compressed data: ");
        for (size_t i = 0; i < compressed_size; i += 2) {
            if (i > 0) printf(" ");
            printf("%02X%02X", compressed[i], compressed[i + 1]);
        }
        printf("\n");
    } else {
        printf("Compression failed or produced no output.\n");
    }
    
    buffer_free(&output);
    return EXIT_SUCCESS;
}