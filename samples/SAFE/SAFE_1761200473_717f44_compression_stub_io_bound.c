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
    size_t capacity = CHUNK_SIZE;
    buf.data = malloc(capacity);
    if (!buf.data) return buf;
    
    size_t total = 0;
    while (1) {
        if (total + CHUNK_SIZE > capacity) {
            if (capacity > MAX_INPUT_SIZE - CHUNK_SIZE) {
                free(buf.data);
                buf.data = NULL;
                return buf;
            }
            capacity *= 2;
            unsigned char* new_data = realloc(buf.data, capacity);
            if (!new_data) {
                free(buf.data);
                buf.data = NULL;
                return buf;
            }
            buf.data = new_data;
        }
        
        size_t bytes_read = fread(buf.data + total, 1, CHUNK_SIZE, stdin);
        total += bytes_read;
        
        if (bytes_read < CHUNK_SIZE) {
            if (ferror(stdin)) {
                free(buf.data);
                buf.data = NULL;
                return buf;
            }
            break;
        }
    }
    
    buf.size = total;
    return buf;
}

int write_output(const Buffer* buf) {
    if (!buf || !buf->data) return 0;
    
    size_t written = 0;
    while (written < buf->size) {
        size_t to_write = buf->size - written;
        if (to_write > CHUNK_SIZE) to_write = CHUNK_SIZE;
        
        size_t bytes_written = fwrite(buf->data + written, 1, to_write, stdout);
        if (bytes_written != to_write) return 0;
        written += bytes_written;
    }
    
    return fflush(stdout) == 0;
}

Buffer compress_stub(const Buffer* input) {
    Buffer output = {NULL, 0};
    if (!input || !input->data || input->size == 0) return output;
    
    size_t max_output_size = input->size + 128;
    output.data = malloc(max_output_size);
    if (!output.data) return output;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        if (out_pos + 3 >= max_output_size) {
            free(output.data);
            output.data = NULL;
            return output;
        }
        
        unsigned char current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               count < 255 && 
               input->data[in_pos + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output.data[out_pos++] = 0xFF;
            output.data[out_pos++] = current;
            output.data[out_pos++] = (unsigned char)count;
            in_pos += count;
        } else {
            output.data[out_pos++] = current;
            in_pos++;
        }
    }
    
    output.size = out_pos;
    return output;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        free(input.data);
        return 1;
    }
    
    Buffer compressed = compress_stub(&input);
    free(input.data);
    
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    if (!write_output(&compressed)) {
        fprintf(stderr, "Error writing output\n");
        free(compressed.data);
        return 1;
    }
    
    free(compressed.data);
    return 0;
}