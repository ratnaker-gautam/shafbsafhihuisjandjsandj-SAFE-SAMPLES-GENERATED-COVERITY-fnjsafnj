//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t capacity) {
    if (capacity == 0 || capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    buf->data = malloc(capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
    }
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf->size + len > buf->capacity) {
        return 0;
    }
    uint8_t *dst = buf->data + buf->size;
    while (len-- > 0) {
        *dst++ = *src++;
    }
    buf->size += (dst - (buf->data + buf->size));
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    
    while (in_ptr < in_end && out_ptr < out_end - 2) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (count >= 3) {
            *out_ptr++ = current;
            *out_ptr++ = (uint8_t)count;
            in_ptr += count;
        } else {
            *out_ptr++ = current;
            in_ptr++;
        }
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    
    while (in_ptr < in_end && out_ptr < out_end) {
        if (in_ptr + 1 < in_end && *(in_ptr + 1) > 2) {
            uint8_t value = *in_ptr;
            uint8_t count = *(in_ptr + 1);
            
            if (out_ptr + count > out_end) {
                break;
            }
            
            uint8_t *fill_ptr = out_ptr;
            while (fill_ptr < out_ptr + count) {
                *fill_ptr++ = value;
            }
            out_ptr += count;
            in_ptr += 2;
        } else {
            *out_ptr++ = *in_ptr++;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    size_t compressed_size = compress_rle(input, input_len, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size < input_len) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
    } else {
        printf("No compression achieved\n");
    }
    
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input, decompressed, input_len) != 0) {
        fprintf(stderr, "