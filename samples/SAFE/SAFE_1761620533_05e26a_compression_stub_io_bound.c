//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

typedef struct {
    unsigned char* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buf = {NULL, 0};
    unsigned char* temp = malloc(MAX_INPUT_SIZE);
    if (!temp) return buf;
    
    size_t total = 0;
    while (total < MAX_INPUT_SIZE) {
        size_t to_read = MAX_INPUT_SIZE - total;
        if (to_read > CHUNK_SIZE) to_read = CHUNK_SIZE;
        
        size_t n = fread(temp + total, 1, to_read, stdin);
        if (n == 0) break;
        total += n;
    }
    
    if (total > 0) {
        buf.data = malloc(total);
        if (buf.data) {
            memcpy(buf.data, temp, total);
            buf.size = total;
        }
    }
    
    free(temp);
    return buf;
}

int write_output(const Buffer* buf) {
    if (!buf || !buf->data || buf->size == 0) return 0;
    
    size_t written = 0;
    while (written < buf->size) {
        size_t to_write = buf->size - written;
        if (to_write > CHUNK_SIZE) to_write = CHUNK_SIZE;
        
        size_t n = fwrite(buf->data + written, 1, to_write, stdout);
        if (n != to_write) return 0;
        written += n;
    }
    
    return 1;
}

Buffer compress_stub(const Buffer* input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0) return output;
    
    size_t max_output = input->size + (input->size / 255) + 16;
    if (max_output < input->size) return output;
    
    unsigned char* compressed = malloc(max_output);
    if (!compressed) return output;
    
    size_t in_pos = 0;
    size_t out_pos = 0;
    
    while (in_pos < input->size) {
        unsigned char current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               count < 255 && 
               input->data[in_pos + count] == current) {
            count++;
        }
        
        if (out_pos + 2 > max_output) {
            free(compressed);
            return output;
        }
        
        compressed[out_pos++] = (unsigned char)count;
        compressed[out_pos++] = current;
        in_pos += count;
    }
    
    output.data = compressed;
    output.size = out_pos;
    return output;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    if (input.size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Error: Input too large\n");
        free(input.data);
        return 1;
    }
    
    Buffer compressed = compress_stub(&input);
    free(input.data);
    
    if (!compressed.data) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    if (!write_output(&compressed)) {
        fprintf(stderr, "Error: Failed to write output\n");
        free(compressed.data);
        return 1;
    }
    
    free(compressed.data);
    return 0;
}