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
            return -1;
        }
        
        memcpy(buf->data + total_read, chunk, chunk_len);
        total_read += chunk_len;
        
        if (chunk_len < CHUNK_SIZE - 1) {
            break;
        }
    }
    
    buf->size = total_read;
    return 0;
}

int compress_data(const Buffer *input, Buffer *output) {
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
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 >= MAX_INPUT_SIZE) {
                return -1;
            }
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
        } else {
            if (out_idx + count >= MAX_INPUT_SIZE) {
                return -1;
            }
            for (size_t i = 0; i < count; i++) {
                output->data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output->size = out_idx;
    return 0;
}

void write_output(const Buffer *buf) {
    for (size_t i = 0; i < buf->size; i++) {
        if (putchar(buf->data[i]) == EOF) {
            break;
        }
    }
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    if (read_input(&input) != 0) {
        fprintf(stderr, "Input too large or read error\n");
        return EXIT_FAILURE;
    }
    
    if (compress_data(&input, &output) != 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    write_output(&output);
    
    return EXIT_SUCCESS;
}