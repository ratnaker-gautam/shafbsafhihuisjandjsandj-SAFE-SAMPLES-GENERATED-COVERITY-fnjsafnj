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
    if (buf == NULL || initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
        return -1;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return -1;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf == NULL || data == NULL || len == 0 || len > SIZE_MAX - buf->size) {
        return -1;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        if (new_capacity > SIZE_MAX / 2) {
            return -1;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return -1;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 0;
}

int compress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t *out_len) {
    if (input == NULL || output == NULL || out_len == NULL || len == 0 || len > CHUNK_SIZE) {
        return -1;
    }
    size_t compressed = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < len && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        if (compressed + 2 > CHUNK_SIZE) {
            return -1;
        }
        output[compressed++] = (uint8_t)count;
        output[compressed++] = current;
        i += count;
    }
    *out_len = compressed;
    return 0;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t total_read = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    int c;
    while (total_read < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_stack[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed;
    if (buffer_init(&compressed, total_read) != 0) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < total_read) {
        size_t chunk_len = total_read - processed;
        if (chunk_len > CHUNK_SIZE) {
            chunk_len = CHUNK_SIZE;
        }
        
        uint8_t chunk_output[CHUNK_SIZE * 2];
        size_t out_len;
        
        if (compress_chunk(input_stack + processed, chunk_len, chunk_output, &out_len) == 0) {
            if (buffer_append(&compressed, chunk_output, out_len) != 0) {
                printf("Compression buffer overflow.\n");
                buffer_free(&compressed);
                return 1;
            }
        } else {
            printf("Chunk compression failed.\n");
            buffer_free(&compressed);
            return 1;
        }
        
        processed += chunk_len;
    }
    
    printf("Original size: %zu bytes\n", total_read);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed.size / total_read * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}