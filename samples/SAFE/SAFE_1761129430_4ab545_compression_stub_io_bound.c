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
        int c = getchar();
        if (c == EOF) break;
        if (c < 0 || c > 255) return 0;
        buf->data[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0) return 0;
    buf->size = total_read;
    return 1;
}

int simple_compress(const Buffer *input, uint8_t *output, size_t *output_size) {
    if (!input || !output || !output_size || input->size == 0) return 0;
    
    size_t in_pos = 0;
    size_t out_pos = 0;
    
    while (in_pos < input->size && out_pos < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               count < 255 && 
               input->data[in_pos + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[out_pos++] = 0xFF;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count && out_pos < MAX_OUTPUT_SIZE; i++) {
                output[out_pos++] = current;
            }
        }
        
        in_pos += count;
    }
    
    if (out_pos >= MAX_OUTPUT_SIZE) return 0;
    *output_size = out_pos;
    return 1;
}

void write_output(const uint8_t *data, size_t size) {
    if (!data || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        if (putchar(data[i]) == EOF) {
            break;
        }
    }
}

int main(void) {
    Buffer input;
    uint8_t output[MAX_OUTPUT_SIZE];
    size_t output_size = 0;
    
    if (!read_input(&input)) {
        return 1;
    }
    
    if (!simple_compress(&input, output, &output_size)) {
        return 1;
    }
    
    write_output(output, output_size);
    return 0;
}