//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    size_t capacity = CHUNK_SIZE;
    buffer.data = malloc(capacity);
    if (!buffer.data) return buffer;
    
    int c;
    while ((c = getchar()) != EOF) {
        if (buffer.size >= MAX_INPUT_SIZE) {
            free(buffer.data);
            buffer.data = NULL;
            return buffer;
        }
        
        if (buffer.size >= capacity) {
            if (capacity > MAX_INPUT_SIZE / 2) {
                free(buffer.data);
                buffer.data = NULL;
                return buffer;
            }
            capacity *= 2;
            if (capacity > MAX_INPUT_SIZE) {
                capacity = MAX_INPUT_SIZE;
            }
            uint8_t* new_data = realloc(buffer.data, capacity);
            if (!new_data) {
                free(buffer.data);
                buffer.data = NULL;
                return buffer;
            }
            buffer.data = new_data;
        }
        
        buffer.data[buffer.size++] = (uint8_t)c;
    }
    
    return buffer;
}

Buffer compress_rle(const Buffer* input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0) return output;
    
    size_t capacity = input->size * 2;
    if (capacity < input->size) {
        return output;
    }
    output.data = malloc(capacity);
    if (!output.data) return output;
    
    size_t i = 0;
    while (i < input->size) {
        if (output.size + 2 > capacity) {
            free(output.data);
            output.data = NULL;
            return output;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && input->data[i + count] == current) {
            count++;
        }
        
        output.data[output.size++] = (uint8_t)count;
        output.data[output.size++] = current;
        i += count;
    }
    
    return output;
}

void write_output(const Buffer* buffer) {
    if (!buffer || !buffer->data) return;
    for (size_t i = 0; i < buffer->size; i++) {
        putchar(buffer->data[i]);
    }
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        return EXIT_FAILURE;
    }
    
    Buffer compressed = compress_rle(&input);
    free(input.data);
    
    if (!compressed.data) {
        return EXIT_FAILURE;
    }
    
    write_output(&compressed);
    free(compressed.data);
    
    return EXIT_SUCCESS;
}