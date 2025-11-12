//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

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
    if (!buf->data) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0) {
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
        if (!new_data) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t *out_len) {
    if (!input || !output || !out_len || len == 0) {
        *out_len = 0;
        return;
    }
    size_t j = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (count > 2 || current == 0xFF) {
            if (j + 3 > CHUNK_SIZE * 2) {
                break;
            }
            output[j++] = 0xFF;
            output[j++] = current;
            output[j++] = (uint8_t)count;
            i += count;
        } else {
            if (j + 1 > CHUNK_SIZE * 2) {
                break;
            }
            output[j++] = current;
            i++;
        }
        if (j >= CHUNK_SIZE * 2 - 3) {
            break;
        }
    }
    *out_len = j;
}

int main(void) {
    uint8_t stack_input[MAX_INPUT_SIZE];
    size_t total_read = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    char input_line[MAX_INPUT_SIZE * 2];
    if (!fgets(input_line, sizeof(input_line), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(stack_input, input_line, input_len);
    total_read = input_len;
    
    if (total_read == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, total_read)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < total_read) {
        size_t chunk_len = total_read - processed;
        if (chunk_len > CHUNK_SIZE) {
            chunk_len = CHUNK_SIZE;
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_len;
        compress_chunk(stack_input + processed, chunk_len, compressed_chunk, &compressed_len);
        
        if (compressed_len > 0) {
            if (!buffer_append(&compressed, compressed_chunk, compressed_len)) {
                fprintf(stderr, "Compression buffer overflow\n");
                buffer_free(&compressed);
                return 1;
            }
        }
        
        processed += chunk_len;
    }
    
    printf("Original size: %zu bytes\n", total_read);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (total_read > 0) ? (100.0 * compressed.size / total_read) : 0.0);
    
    printf("Compressed data (