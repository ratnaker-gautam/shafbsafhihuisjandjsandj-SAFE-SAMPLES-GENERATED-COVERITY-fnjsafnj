//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 4096

typedef struct {
    uint8_t data[BUFFER_SIZE];
    size_t size;
} Buffer;

int read_input(Buffer *buf) {
    if (buf == NULL) return -1;
    
    size_t total_read = 0;
    while (total_read < BUFFER_SIZE) {
        int c = getchar();
        if (c == EOF) break;
        if (c < 0 || c > 255) return -1;
        buf->data[total_read++] = (uint8_t)c;
    }
    
    buf->size = total_read;
    return 0;
}

int compress_rle(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL) return -1;
    if (input->size == 0) return -1;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < BUFFER_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               count < 255 && 
               input->data[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= BUFFER_SIZE) return -1;
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        in_idx += count;
    }
    
    output->size = out_idx;
    return 0;
}

void write_output(const Buffer *buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    if (read_input(&input) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        return EXIT_FAILURE;
    }
    
    if (compress_rle(&input, &output) != 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    write_output(&output);
    return EXIT_SUCCESS;
}