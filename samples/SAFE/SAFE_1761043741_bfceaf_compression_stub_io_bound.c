//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int read_input(Buffer *buf) {
    if (!buf) return 0;
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        int ch = getchar();
        if (ch == EOF) break;
        if (ch < 0 || ch > 255) return 0;
        buf->data[total_read++] = (uint8_t)ch;
    }
    
    buf->size = total_read;
    return 1;
}

int simple_compress(const Buffer *input, Buffer *output) {
    if (!input || !output || input->size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && 
               input->data[i + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output->data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    output->size = out_idx;
    return out_idx < MAX_OUTPUT_SIZE;
}

void write_output(const Buffer *buf) {
    if (!buf) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    if (!read_input(&input)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        return 1;
    }
    
    if (!simple_compress(&input, &output)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    write_output(&output);
    
    return 0;
}