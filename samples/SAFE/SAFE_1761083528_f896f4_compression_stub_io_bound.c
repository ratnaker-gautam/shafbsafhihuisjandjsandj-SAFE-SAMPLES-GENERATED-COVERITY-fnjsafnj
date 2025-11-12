//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 4096
#define MAX_FILENAME 256

typedef struct {
    uint8_t data[BUFFER_SIZE];
    size_t size;
} Buffer;

int read_file(const char* filename, Buffer* buffer) {
    if (!filename || !buffer) return -1;
    
    FILE* file = fopen(filename, "rb");
    if (!file) return -1;
    
    buffer->size = fread(buffer->data, 1, BUFFER_SIZE, file);
    int result = ferror(file) ? -1 : 0;
    fclose(file);
    
    return result;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (!filename || !buffer || buffer->size > BUFFER_SIZE) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    int result = (written == buffer->size && !ferror(file)) ? 0 : -1;
    fclose(file);
    
    return result;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (!input || !output || input->size == 0) {
        output->size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < BUFFER_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_idx + 2 >= BUFFER_SIZE) break;
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (!input || !output || input->size == 0) {
        output->size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx + 1 < input->size && out_idx < BUFFER_SIZE) {
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > BUFFER_SIZE) {
            count = BUFFER_SIZE - out_idx;
        }
        
        for (uint8_t i = 0; i < count && out_idx < BUFFER_SIZE; i++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <compress|decompress> <input> <output>\n", argv[0]);
        return 1;
    }
    
    const char* operation = argv[1];
    const char* input_file = argv[2];
    const char* output_file = argv[3];
    
    if (strlen(input_file) >= MAX_FILENAME || strlen(output_file) >= MAX_FILENAME) {
        fprintf(stderr, "Filename too long\n");
        return 1;
    }
    
    Buffer input, output;
    
    if (read_file(input_file, &input) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    int is_compress = strcmp(operation, "compress") == 0;
    int is_decompress = strcmp(operation, "decompress") == 0;
    
    if (!is_compress && !is_decompress) {
        fprintf(stderr, "Operation must be 'compress' or 'decompress'\n");
        return 1;
    }
    
    if (is_compress) {
        simple_compress(&input, &output);
    } else {
        simple_decompress(&input, &output);
    }
    
    if (write_file(output_file, &output) != 0) {
        fprintf(stderr, "Error writing output file\n");
        return 1;
    }
    
    printf("Operation completed successfully\n");
    printf("Input size: %zu bytes\n", input.size);
    printf("Output size: %zu bytes\n", output.size);
    
    return 0;
}