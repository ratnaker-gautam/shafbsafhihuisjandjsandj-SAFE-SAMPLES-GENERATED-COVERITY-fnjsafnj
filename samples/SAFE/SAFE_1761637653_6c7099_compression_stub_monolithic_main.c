//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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
    Buffer buf = {NULL, 0};
    size_t capacity = 4096;
    buf.data = malloc(capacity);
    if (!buf.data) return buf;
    
    int c;
    while ((c = getchar()) != EOF) {
        if (buf.size >= capacity) {
            if (capacity >= MAX_INPUT_SIZE) break;
            capacity *= 2;
            if (capacity > MAX_INPUT_SIZE) capacity = MAX_INPUT_SIZE;
            uint8_t *new_data = realloc(buf.data, capacity);
            if (!new_data) break;
            buf.data = new_data;
        }
        buf.data[buf.size++] = (uint8_t)c;
    }
    return buf;
}

Buffer compress_stub(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0) return output;
    
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (!output.data) return output;
    
    size_t in_pos = 0;
    while (in_pos < input->size && output.size < MAX_OUTPUT_SIZE - 5) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               count < 255 && 
               input->data[in_pos + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output.data[output.size++] = 0xFF;
            output.data[output.size++] = current;
            output.data[output.size++] = (uint8_t)count;
            in_pos += count;
        } else {
            output.data[output.size++] = current;
            in_pos++;
        }
    }
    
    return output;
}

void write_output(const Buffer *output) {
    if (!output || !output->data || output->size == 0) return;
    for (size_t i = 0; i < output->size; i++) {
        putchar(output->data[i]);
    }
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        return EXIT_FAILURE;
    }
    
    Buffer compressed = compress_stub(&input);
    if (!compressed.data) {
        free(input.data);
        return EXIT_FAILURE;
    }
    
    write_output(&compressed);
    
    free(input.data);
    free(compressed.data);
    return EXIT_SUCCESS;
}