//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

typedef struct {
    uint8_t stack_data[CHUNK_SIZE];
    size_t stack_used;
    Buffer *heap_buffer;
} Compressor;

Buffer* create_buffer(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > 65536) {
        return NULL;
    }
    
    Buffer *buf = malloc(sizeof(Buffer));
    if (!buf) return NULL;
    
    buf->data = malloc(initial_capacity);
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    
    buf->size = 0;
    buf->capacity = initial_capacity;
    return buf;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity > 65536) new_capacity = 65536;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void free_buffer(Buffer *buf) {
    if (buf) {
        free(buf->data);
        free(buf);
    }
}

int compress_chunk(Compressor *comp, const uint8_t *input, size_t input_size) {
    if (!comp || !input || input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    
    size_t stack_available = CHUNK_SIZE - comp->stack_used;
    if (input_size <= stack_available) {
        memcpy(comp->stack_data + comp->stack_used, input, input_size);
        comp->stack_used += input_size;
        return 1;
    }
    
    if (!comp->heap_buffer) {
        comp->heap_buffer = create_buffer(256);
        if (!comp->heap_buffer) return 0;
    }
    
    if (comp->stack_used > 0) {
        if (!buffer_append(comp->heap_buffer, comp->stack_data, comp->stack_used)) {
            return 0;
        }
        comp->stack_used = 0;
    }
    
    return buffer_append(comp->heap_buffer, input, input_size);
}

void init_compressor(Compressor *comp) {
    if (comp) {
        comp->stack_used = 0;
        comp->heap_buffer = NULL;
    }
}

void cleanup_compressor(Compressor *comp) {
    if (comp) {
        free_buffer(comp->heap_buffer);
    }
}

int main() {
    Compressor compressor;
    init_compressor(&compressor);
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    uint8_t input[MAX_INPUT_SIZE];
    if (!fgets((char*)input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len-1] == '\n') {
        input[input_len-1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Processing %zu bytes...\n", input_len);
    
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        if (!compress_chunk(&compressor, input + processed, chunk_size)) {
            printf("Compression failed\n");
            cleanup_compressor(&compressor);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    printf("Compression completed successfully\n");
    printf("Stack usage: %zu bytes\n", compressor.stack_used);
    printf("Heap usage: %zu bytes\n", compressor.heap_buffer ? compressor.heap_buffer->size : 0);
    
    cleanup_compressor(&compressor);
    return 0;
}