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
    char chunk[CHUNK_SIZE];
    
    while (total_read < MAX_INPUT_SIZE) {
        if (fgets(chunk, sizeof(chunk), stdin) == NULL) {
            break;
        }
        
        size_t chunk_len = strlen(chunk);
        if (total_read + chunk_len >= MAX_INPUT_SIZE) {
            chunk_len = MAX_INPUT_SIZE - total_read - 1;
        }
        
        if (chunk_len > 0) {
            memcpy(buf->data + total_read, chunk, chunk_len);
            total_read += chunk_len;
        }
        
        if (chunk_len < CHUNK_SIZE - 1) {
            break;
        }
    }
    
    buf->size = total_read;
    if (total_read > 0 && buf->data[total_read - 1] == '\n') {
        buf->size--;
    }
    
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
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == '\\') {
            if (out_idx + 3 >= MAX_INPUT_SIZE) break;
            output->data[out_idx++] = '\\';
            output->data[out_idx++] = (uint8_t)count;
            output->data[out_idx++] = current;
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
    for (size_t i = 0; i < buf->size; i++) {
        if (putchar(buf->data[i]) == EOF) {
            return -1;
        }
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
    
    if (input.size >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    simple_compress(&input, &output);
    
    if (write_output(&output) != 0) {
        fprintf(stderr, "Error writing output\n");
        return 1;
    }
    
    return 0;
}