//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
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
        if (ch < 0 || ch > 255) continue;
        
        buf->data[total_read] = (uint8_t)ch;
        total_read++;
    }
    
    buf->size = total_read;
    return 1;
}

int simple_compress(const Buffer *input, Buffer *output) {
    if (!input || !output || input->size == 0) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size && out_pos < MAX_OUTPUT_SIZE - 5) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_pos + 3 >= MAX_OUTPUT_SIZE) break;
            output->data[out_pos++] = 0xFF;
            output->data[out_pos++] = current;
            output->data[out_pos++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count && out_pos < MAX_OUTPUT_SIZE; i++) {
                output->data[out_pos++] = current;
            }
        }
        
        in_pos += count;
    }
    
    output->size = out_pos;
    return 1;
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
        fprintf(stderr, "Error reading input\n");
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
    
    if (output.size > 0) {
        fprintf(stderr, "Compressed %zu bytes to %zu bytes\n", input.size, output.size);
    }
    
    return 0;
}