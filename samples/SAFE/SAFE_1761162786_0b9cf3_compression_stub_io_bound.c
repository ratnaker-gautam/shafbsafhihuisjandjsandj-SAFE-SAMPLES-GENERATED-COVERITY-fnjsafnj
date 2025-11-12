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

int read_file(const char* filename, Buffer* buffer) {
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

int write_file(const char* filename, const Buffer* buffer) {
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
    size_t i = 0;
    
    while (i < input->size && output->size < BUFFER_SIZE - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && 
               input->data[i + count] == current) {
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
            for (size_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
                output->data[output->size++] = current;
            }
        }
        
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size && output->size < BUFFER_SIZE) {
        if (input->data[i] == 0xFF && i + 2 < input->size) {
            uint8_t value = input->data[i + 1];
            uint8_t count = input->data[i + 2];
            
            for (uint8_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
                output->data[output->size++] = value;
            }
            i += 3;
        } else {
            output->data[output->size++] = input->data[i++];
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <compress|decompress> <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    const char* operation = argv[1];
    const char* input_filename = argv[2];
    const char* output_filename = argv[3];
    
    if (strlen(input_filename) >= MAX_FILENAME || strlen(output_filename) >= MAX_FILENAME) {
        fprintf(stderr, "Filename too long\n");
        return 1;
    }
    
    Buffer input, output;
    
    if (read_file(input_filename, &input) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    if (strcmp(operation, "compress") == 0) {
        simple_compress(&input, &output);
    } else if (strcmp(operation, "decompress") == 0) {
        simple_decompress(&input, &output);
    } else {
        fprintf(stderr, "Invalid operation. Use 'compress' or 'decompress'\n");
        return 1;
    }
    
    if (write_file(output_filename, &output) != 0) {
        fprintf(stderr, "Error writing output file\n");
        return 1;
    }
    
    printf("Operation completed successfully\n");
    printf("Input size: %zu bytes\n", input.size);
    printf("Output size: %zu bytes\n", output.size);
    
    return 0;
}