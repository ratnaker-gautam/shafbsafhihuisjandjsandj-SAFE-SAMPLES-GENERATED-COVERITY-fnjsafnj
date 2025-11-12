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
    size_t total_read = 0;
    size_t bytes_read;
    
    while (total_read < MAX_INPUT_SIZE) {
        bytes_read = fread(buf->data + total_read, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            if (ferror(stdin)) return -1;
        }
        total_read += bytes_read;
        if (bytes_read < CHUNK_SIZE) break;
    }
    
    if (total_read == 0 && !feof(stdin)) return -1;
    
    buf->size = total_read;
    return 0;
}

int compress_rle(const Buffer *input, Buffer *output) {
    if (input->size == 0) {
        output->size = 0;
        return 0;
    }
    
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
        
        if (out_idx + 2 >= MAX_INPUT_SIZE) return -1;
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
    return 0;
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    
    if (read_input(&input) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (input.size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }
    
    if (compress_rle(&input, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t written = fwrite(compressed.data, 1, compressed.size, stdout);
    if (written != compressed.size) {
        fprintf(stderr, "Error writing output\n");
        return EXIT_FAILURE;
    }
    
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Error flushing output\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}