//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    uint8_t chunk[CHUNK_SIZE];
    size_t total_size = 0;
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (total_size + bytes_read > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            free(buffer.data);
            exit(1);
        }
        
        uint8_t* new_data = realloc(buffer.data, total_size + bytes_read);
        if (!new_data) {
            fprintf(stderr, "Memory allocation failed\n");
            free(buffer.data);
            exit(1);
        }
        
        buffer.data = new_data;
        memcpy(buffer.data + total_size, chunk, bytes_read);
        total_size += bytes_read;
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Read error\n");
        free(buffer.data);
        exit(1);
    }
    
    buffer.size = total_size;
    return buffer;
}

void compress_data(const Buffer* input, Buffer* output) {
    if (input->size == 0) {
        output->data = NULL;
        output->size = 0;
        return;
    }
    
    size_t max_output_size = input->size + (input->size / 255) + 1;
    output->data = malloc(max_output_size);
    if (!output->data) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_pos + 2 > max_output_size) {
            fprintf(stderr, "Output buffer overflow\n");
            free(output->data);
            exit(1);
        }
        
        output->data[out_pos++] = (uint8_t)count;
        output->data[out_pos++] = current;
        in_pos += count;
    }
    
    output->size = out_pos;
}

int main(void) {
    Buffer input = read_input();
    Buffer compressed;
    
    compress_data(&input, &compressed);
    
    if (fwrite(compressed.data, 1, compressed.size, stdout) != compressed.size) {
        fprintf(stderr, "Write error\n");
        free(input.data);
        free(compressed.data);
        return 1;
    }
    
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Flush error\n");
        free(input.data);
        free(compressed.data);
        return 1;
    }
    
    free(input.data);
    free(compressed.data);
    return 0;
}