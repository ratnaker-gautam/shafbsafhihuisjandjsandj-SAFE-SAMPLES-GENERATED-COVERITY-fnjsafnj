//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) {
        return 0;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                return 0;
            }
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return;
    }
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        uint8_t chunk[2] = {current, (uint8_t)count};
        if (!buffer_append(output, chunk, 2)) {
            return;
        }
        i += count;
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    printf("Enter data to compress (max %zu bytes): ", max_size);
    fflush(stdout);
    if (fgets((char*)buffer, max_size, stdin) == NULL) {
        return 0;
    }
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    return len;
}

void print_compressed(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL || buf->size == 0) {
        return;
    }
    printf("Compressed data (%zu bytes):\n", buf->size);
    for (size_t i = 0; i < buf->size; i += 2) {
        if (i + 1 < buf->size) {
            printf("0x%02x[%d] ", buf->data[i], buf->data[i + 1]);
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    Buffer output_heap;
    
    if (!buffer_init(&output_heap, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        return 1;
    }
    
    int input_len = read_input(input_stack, MAX_INPUT_SIZE);
    if (input_len <= 0) {
        fprintf(stderr, "No input received\n");
        buffer_free(&output_heap);
        return 1;
    }
    
    simple_compress(input_stack, input_len, &output_heap);
    
    if (output_heap.size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&output_heap);
        return 1;
    }
    
    print_compressed(&output_heap);
    
    double ratio = (double)output_heap.size / input_len;
    printf("Compression ratio: %.2f\n", ratio);
    
    buffer_free(&output_heap);
    return 0;
}