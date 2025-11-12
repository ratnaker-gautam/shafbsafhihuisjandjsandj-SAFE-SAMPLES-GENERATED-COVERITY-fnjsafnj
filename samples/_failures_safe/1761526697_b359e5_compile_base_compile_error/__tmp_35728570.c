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
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

int compress_run_length(const unsigned char *input, size_t input_len, 
                       unsigned char *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return 0;
    if (input_len == 0) {
        *output_len = 0;
        return 1;
    }

    unsigned char *out_ptr = output;
    const unsigned char *in_ptr = input;
    const unsigned char *end_ptr = input + input_len;

    while (in_ptr < end_ptr) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        
        const unsigned char *next_ptr = in_ptr + 1;
        while (next_ptr < end_ptr && *next_ptr == current && count < UCHAR_MAX) {
            count++;
            next_ptr++;
        }

        if (out_ptr + 2 > output + *output_len) return 0;
        
        *out_ptr++ = (unsigned char)count;
        *out_ptr++ = current;
        
        in_ptr += count;
    }

    *output_len = out_ptr - output;
    return 1;
}

int main(void) {
    Buffer input_buf, output_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&output_buf, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }

    printf("Enter text to compress (max %zu characters): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }

    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }

    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 0;
    }

    if (!buffer_append(&input_buf, (unsigned char*)input_line, input_len)) {
        fprintf(stderr, "Failed to store input\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }

    size_t output_capacity = output_buf.capacity;
    if (!compress_run_length(input_buf.data, input_buf.size, 
                           output_buf.data, &output_capacity)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }

    output_buf.size = output_capacity;

    printf("Original: %s\n", input_line);
    printf("Compressed size: %zu bytes\n", output_buf.size);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output_buf.size; i += 2) {
        if (i + 1 < output_buf.size) {
            printf("[%u '%c'] ", output_buf.data[i], 
                   output_buf.data[i + 1] >= 32 && output_buf.data[i + 1] <= 126 ? 
                   output_buf.data[i + 1] : '.');
        }
    }
    printf("\n");

    double ratio = (double)output_buf.size / (double)input_buf.size;
    printf("Compression ratio: %.2f\n", ratio);