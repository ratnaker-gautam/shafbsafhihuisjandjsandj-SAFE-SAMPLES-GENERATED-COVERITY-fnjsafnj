//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int read_input(Buffer *buf) {
    if (!buf) return 0;
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        size_t to_read = MAX_INPUT_SIZE - total_read;
        if (to_read > CHUNK_SIZE) to_read = CHUNK_SIZE;
        
        size_t bytes_read = fread(buf->data + total_read, 1, to_read, stdin);
        if (bytes_read == 0) break;
        
        total_read += bytes_read;
        
        if (feof(stdin)) break;
        if (ferror(stdin)) return 0;
    }
    
    buf->size = total_read;
    return 1;
}

int compress_rle(const Buffer *input, Buffer *output) {
    if (!input || !output || input->size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_INPUT_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               count < 255 && 
               input->data[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= MAX_INPUT_SIZE) return 0;
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        in_idx += count;
    }
    
    output->size = out_idx;
    return 1;
}

int write_output(const Buffer *buf) {
    if (!buf) return 0;
    
    size_t total_written = 0;
    while (total_written < buf->size) {
        size_t to_write = buf->size - total_written;
        if (to_write > CHUNK_SIZE) to_write = CHUNK_SIZE;
        
        size_t bytes_written = fwrite(buf->data + total_written, 1, to_write, stdout);
        if (bytes_written != to_write) return 0;
        
        total_written += bytes_written;
    }
    
    return fflush(stdout) == 0;
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
    
    if (!compress_rle(&input, &output)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    if (!write_output(&output)) {
        fprintf(stderr, "Error writing output\n");
        return 1;
    }
    
    return 0;
}