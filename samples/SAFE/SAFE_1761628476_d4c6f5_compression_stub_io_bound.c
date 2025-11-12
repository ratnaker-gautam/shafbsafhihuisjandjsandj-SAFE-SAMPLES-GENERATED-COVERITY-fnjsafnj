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
            break;
        }
        total_read += bytes_read;
    }
    
    if (ferror(stdin)) {
        return -1;
    }
    
    buf->size = total_read;
    return 0;
}

void simple_compress(const Buffer *input, Buffer *output) {
    if (input->size == 0) {
        output->size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_INPUT_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < UINT8_MAX) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 >= MAX_INPUT_SIZE) break;
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count && out_idx < MAX_INPUT_SIZE; i++) {
                output->data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output->size = out_idx;
}

int write_output(const Buffer *buf) {
    size_t total_written = 0;
    size_t bytes_written;
    
    while (total_written < buf->size) {
        size_t to_write = buf->size - total_written;
        if (to_write > CHUNK_SIZE) {
            to_write = CHUNK_SIZE;
        }
        
        bytes_written = fwrite(buf->data + total_written, 1, to_write, stdout);
        if (bytes_written == 0) {
            if (ferror(stdout)) {
                return -1;
            }
            break;
        }
        total_written += bytes_written;
    }
    
    return 0;
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    if (read_input(&input) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    simple_compress(&input, &output);
    
    if (write_output(&output) != 0) {
        fprintf(stderr, "Error writing output\n");
        return 1;
    }
    
    return 0;
}