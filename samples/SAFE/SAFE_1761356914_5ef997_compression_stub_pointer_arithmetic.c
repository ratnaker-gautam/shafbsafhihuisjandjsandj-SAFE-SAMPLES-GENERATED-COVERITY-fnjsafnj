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

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (buf->size + len > new_capacity) {
            new_capacity = buf->size + len;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

void compress_run_length(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        unsigned char compressed[2] = {value, (unsigned char)count};
        if (!buffer_append(output, compressed, 2)) {
            return;
        }
        
        current = next;
    }
}

int main(void) {
    Buffer input_buf, output_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Error: Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&output_buf, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Error: Failed to initialize output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        
        if (total_read < input_buf.capacity) {
            input_buf.data[total_read] = (unsigned char)c;
            total_read++;
        } else {
            break;
        }
    }
    
    input_buf.size = total_read;
    
    if (total_read == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 0;
    }
    
    compress_run_length(input_buf.data, input_buf.size, &output_buf);
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", output_buf.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)output_buf.size / input_buf.size) * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output_buf.size; i += 2) {
        if (i + 1 < output_buf.size) {
            printf("(%d,%d) ", output_buf.data[i], output_buf.data[i + 1]);
        }
    }
    printf("\n");
    
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    
    return 0;
}