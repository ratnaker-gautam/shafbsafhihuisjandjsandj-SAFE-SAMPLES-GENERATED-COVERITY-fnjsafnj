//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define MAX_OUTPUT_SIZE 2097152

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    size_t capacity = 4096;
    buffer.data = malloc(capacity);
    if (!buffer.data) return buffer;
    
    size_t pos = 0;
    int c;
    while ((c = getchar()) != EOF) {
        if (pos >= capacity) {
            if (capacity >= MAX_INPUT_SIZE) {
                free(buffer.data);
                buffer.data = NULL;
                return buffer;
            }
            capacity *= 2;
            if (capacity > MAX_INPUT_SIZE) capacity = MAX_INPUT_SIZE;
            uint8_t *new_data = realloc(buffer.data, capacity);
            if (!new_data) {
                free(buffer.data);
                buffer.data = NULL;
                return buffer;
            }
            buffer.data = new_data;
        }
        buffer.data[pos++] = (uint8_t)c;
    }
    buffer.size = pos;
    return buffer;
}

int write_output(const Buffer *buffer) {
    if (!buffer || !buffer->data) return 0;
    
    for (size_t i = 0; i < buffer->size; i++) {
        if (putchar(buffer->data[i]) == EOF) return 0;
    }
    return 1;
}

Buffer compress_stub(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0) return output;
    
    size_t max_output = input->size * 2;
    if (max_output > MAX_OUTPUT_SIZE) max_output = MAX_OUTPUT_SIZE;
    
    output.data = malloc(max_output);
    if (!output.data) return output;
    
    size_t in_pos = 0;
    size_t out_pos = 0;
    
    while (in_pos < input->size && out_pos < max_output - 2) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3) {
            output.data[out_pos++] = 0xFF;
            output.data[out_pos++] = current;
            output.data[out_pos++] = (uint8_t)count;
            in_pos += count;
        } else {
            for (size_t i = 0; i < count && out_pos < max_output; i++) {
                output.data[out_pos++] = input->data[in_pos++];
            }
        }
    }
    
    output.size = out_pos;
    return output;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    if (input.size == 0) {
        free(input.data);
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    Buffer compressed = compress_stub(&input);
    free(input.data);
    
    if (!compressed.data) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    if (!write_output(&compressed)) {
        free(compressed.data);
        fprintf(stderr, "Error: Failed to write output\n");
        return 1;
    }
    
    free(compressed.data);
    return 0;
}