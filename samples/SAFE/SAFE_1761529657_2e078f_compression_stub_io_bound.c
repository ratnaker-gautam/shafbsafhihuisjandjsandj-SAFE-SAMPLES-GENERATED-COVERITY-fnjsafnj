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

int read_input_file(const char *filename, Buffer *buffer) {
    if (filename == NULL || buffer == NULL) return -1;
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return -1;
    
    buffer->size = fread(buffer->data, 1, BUFFER_SIZE, file);
    if (ferror(file)) {
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
}

int write_output_file(const char *filename, const Buffer *buffer) {
    if (filename == NULL || buffer == NULL) return -1;
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    if (written != buffer->size || ferror(file)) {
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
}

void simple_compress(const Buffer *input, Buffer *output) {
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (strlen(argv[1]) >= MAX_FILENAME || strlen(argv[2]) >= MAX_FILENAME) {
        fprintf(stderr, "Filename too long\n");
        return EXIT_FAILURE;
    }
    
    Buffer input_buffer = {0};
    Buffer output_buffer = {0};
    
    if (read_input_file(argv[1], &input_buffer) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return EXIT_FAILURE;
    }
    
    if (input_buffer.size == 0) {
        fprintf(stderr, "Input file is empty\n");
        return EXIT_FAILURE;
    }
    
    simple_compress(&input_buffer, &output_buffer);
    
    if (output_buffer.size == 0) {
        fprintf(stderr, "Compression produced no output\n");
        return EXIT_FAILURE;
    }
    
    if (write_output_file(argv[2], &output_buffer) != 0) {
        fprintf(stderr, "Error writing output file\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression complete: %zu bytes -> %zu bytes\n", 
           input_buffer.size, output_buffer.size);
    
    return EXIT_SUCCESS;
}