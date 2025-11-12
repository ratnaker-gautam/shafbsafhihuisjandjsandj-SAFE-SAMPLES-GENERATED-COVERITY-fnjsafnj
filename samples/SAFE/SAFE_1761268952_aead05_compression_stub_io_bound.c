//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
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

int read_input_file(const char* filename, Buffer* buffer) {
    if (filename == NULL || buffer == NULL) return -1;
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return -1;
    
    buffer->size = 0;
    size_t total_read = 0;
    
    while (!feof(file) && total_read < BUFFER_SIZE) {
        size_t bytes_read = fread(buffer->data + total_read, 1, BUFFER_SIZE - total_read, file);
        if (bytes_read == 0) break;
        total_read += bytes_read;
    }
    
    buffer->size = total_read;
    fclose(file);
    return 0;
}

int write_output_file(const char* filename, const Buffer* buffer) {
    if (filename == NULL || buffer == NULL) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    fclose(file);
    
    return (written == buffer->size) ? 0 : -1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) return;
    
    output->size = 0;
    size_t input_pos = 0;
    
    while (input_pos < input->size && output->size < BUFFER_SIZE - 2) {
        uint8_t current = input->data[input_pos];
        size_t count = 1;
        
        while (input_pos + count < input->size && 
               count < 255 && 
               input->data[input_pos + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output->data[output->size++] = 0xFF;
            if (output->size < BUFFER_SIZE) {
                output->data[output->size++] = current;
            }
            if (output->size < BUFFER_SIZE) {
                output->data[output->size++] = (uint8_t)count;
            }
        } else {
            for (size_t i = 0; i < count && output->size < BUFFER_SIZE; i++) {
                output->data[output->size++] = current;
            }
        }
        
        input_pos += count;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    if (strlen(argv[1]) >= MAX_FILENAME || strlen(argv[2]) >= MAX_FILENAME) {
        fprintf(stderr, "Filename too long\n");
        return 1;
    }
    
    Buffer input_data;
    Buffer output_data;
    
    if (read_input_file(argv[1], &input_data) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    if (input_data.size == 0) {
        fprintf(stderr, "Input file is empty\n");
        return 1;
    }
    
    simple_compress(&input_data, &output_data);
    
    if (write_output_file(argv[2], &output_data) != 0) {
        fprintf(stderr, "Error writing output file\n");
        return 1;
    }
    
    printf("Compression complete: %zu bytes -> %zu bytes\n", input_data.size, output_data.size);
    return 0;
}