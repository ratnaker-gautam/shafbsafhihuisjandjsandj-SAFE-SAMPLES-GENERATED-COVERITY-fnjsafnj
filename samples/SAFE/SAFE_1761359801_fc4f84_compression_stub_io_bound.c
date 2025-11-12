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
    if (buf == NULL) return 0;
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF) break;
        if (c == '\n') break;
        buf->data[total_read++] = (uint8_t)c;
    }
    buf->size = total_read;
    return 1;
}

int simple_compress(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE - 5) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == '\\' || current == '|') {
            if (out_idx + 5 >= MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = '\\';
            output->data[out_idx++] = '|';
            output->data[out_idx++] = (uint8_t)count;
            output->data[out_idx++] = current;
            output->data[out_idx++] = '|';
        } else {
            for (size_t i = 0; i < count; i++) {
                if (out_idx >= MAX_OUTPUT_SIZE) return 0;
                output->data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output->size = out_idx;
    return 1;
}

void write_output(const Buffer *buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
    putchar('\n');
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    if (!simple_compress(&input, &output)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu -> %zu bytes): ", input.size, output.size);
    write_output(&output);
    
    double ratio = (input.size > 0) ? (double)output.size / input.size : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}