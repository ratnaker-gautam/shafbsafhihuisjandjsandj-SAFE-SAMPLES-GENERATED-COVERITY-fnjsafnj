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
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            return -1;
        }
        
        if (total_read + bytes_read > MAX_INPUT_SIZE) {
            return -1;
        }
        
        memcpy(buf->data + total_read, chunk, bytes_read);
        total_read += bytes_read;
    }
    
    buf->size = total_read;
    return 0;
}

int simple_compress(const Buffer *input, Buffer *output) {
    if (input->size == 0) {
        output->size = 0;
        return 0;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size && out_pos < MAX_INPUT_SIZE - 2) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_pos + 3 > MAX_INPUT_SIZE) return -1;
            output->data[out_pos++] = 0xFF;
            output->data[out_pos++] = current;
            output->data[out_pos++] = (uint8_t)count;
        } else {
            if (out_pos + count > MAX_INPUT_SIZE) return -1;
            for (size_t i = 0; i < count; i++) {
                output->data[out_pos++] = current;
            }
        }
        
        in_pos += count;
    }
    
    output->size = out_pos;
    return 0;
}

int write_output(const Buffer *buf) {
    size_t written = fwrite(buf->data, 1, buf->size, stdout);
    if (written != buf->size) {
        return -1;
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
    
    if (simple_compress(&input, &output) != 0) {
        fprintf(stderr, "Error during compression\n");
        return 1;
    }
    
    if (write_output(&output) != 0) {
        fprintf(stderr, "Error writing output\n");
        return 1;
    }
    
    return 0;
}