//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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
    
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src_end = src + len;
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

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end && out_ptr + 2 <= out_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        *out_ptr = current;
        out_ptr++;
        *out_ptr = (uint8_t)count;
        out_ptr++;
        
        in_ptr += count;
    }
    
    return out_ptr - output;
}

int main(void) {
    char input_buf[MAX_INPUT_SIZE];
    Buffer output_buf;
    buffer_init(&output_buf, 128);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_buf, sizeof(input_buf), stdin) == NULL) {
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buf);
    if (input_len > 0 && input_buf[input_len - 1] == '\n') {
        input_buf[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        buffer_free(&output_buf);
        return EXIT_SUCCESS;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle((uint8_t*)input_buf, input_len, compressed, sizeof(compressed));
    
    if (compressed_size > 0) {
        buffer_append(&output_buf, compressed, compressed_size);
        
        printf("Original size: %zu bytes\n", input_len);
        printf("Compressed size: %zu bytes\n", compressed_size);
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
        
        printf("Compressed data: ");
        const uint8_t *ptr = output_buf.data;
        const uint8_t *end = ptr + output_buf.size;
        while (ptr < end) {
            printf("%02X%02X ", *ptr, *(ptr + 1));
            ptr += 2;
        }
        printf("\n");
    } else {
        printf("Compression failed or produced no output.\n");
    }
    
    buffer_free(&output_buf);
    return EXIT_SUCCESS;
}