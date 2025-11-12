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

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
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
        if (buf->size + len > new_capacity) {
            return;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_end = src + len;
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

size_t simple_compress(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (in_ptr < in_end && out_ptr + 2 < out_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        
        const unsigned char *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (count > 3) {
            *out_ptr = current;
            out_ptr++;
            *out_ptr = (unsigned char)count;
            out_ptr++;
            in_ptr += count;
        } else {
            *out_ptr = current;
            out_ptr++;
            in_ptr++;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    char user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    buffer_init(&compressed, input_len);
    
    unsigned char *temp_output = malloc(input_len * 2);
    if (temp_output == NULL) {
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = simple_compress((unsigned char*)user_input, input_len, temp_output, input_len * 2);
    buffer_append(&compressed, temp_output, compressed_size);
    free(temp_output);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_len) * 100);
    
    printf("Compressed data: ");
    unsigned char *ptr = compressed.data;
    unsigned char *end = compressed.data + compressed.size;
    while (ptr < end) {
        printf("%02X ", *ptr);
        ptr++;
    }
    printf("\n");
    
    buffer_free(&compressed);
    return EXIT_SUCCESS;
}