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
    
    buffer->size = fread(buffer->data, 1, BUFFER_SIZE, file);
    int result = ferror(file) ? -1 : 0;
    fclose(file);
    return result;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (filename == NULL || buffer == NULL) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    int result = (written != buffer->size || ferror(file)) ? -1 : 0;
    fclose(file);
    return result;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size && output->size < BUFFER_SIZE - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && 
               input->data[i + count] == current && 
               count < 255 && 
               output->size < BUFFER_SIZE - 2) {
            count++;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i + 1 < input->size && output->size < BUFFER_SIZE) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (output->size + count > BUFFER_SIZE) {
            count = BUFFER_SIZE - output->size;
        }
        
        for (uint8_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
            output->data[output->size++] = value;
        }
        
        i += 2;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <compress|decompress> <input_file> <output_file>\n", 
                argv[0] ? argv[0] : "program");
        return EXIT_FAILURE;
    }
    
    if (strlen(argv[1]) >= MAX_FILENAME || 
        strlen(argv[2]) >= MAX_FILENAME || 
        strlen(argv[3]) >= MAX_FILENAME) {
        fprintf(stderr, "Error: Filename too long\n");
        return EXIT_FAILURE;
    }
    
    Buffer input, output;
    
    if (read_file(argv[2], &input) != 0) {
        fprintf(stderr, "Error: Cannot read input file\n");
        return EXIT_FAILURE;
    }
    
    if (strcmp(argv[1], "compress") == 0) {
        simple_compress(&input, &output);
    } else if (strcmp(argv[1], "decompress") == 0) {
        simple_decompress(&input, &output);
    } else {
        fprintf(stderr, "Error: Invalid operation. Use 'compress' or 'decompress'\n");
        return EXIT_FAILURE;
    }
    
    if (output.size == 0) {
        fprintf(stderr, "Error: Processing resulted in empty output\n");
        return EXIT_FAILURE;
    }
    
    if (write_file(argv[3], &output) != 0) {
        fprintf(stderr, "Error: Cannot write output file\n");
        return EXIT_FAILURE;
    }
    
    printf("Operation completed successfully\n");
    printf("Input size: %zu bytes\n", input.size);
    printf("Output size: %zu bytes\n", output.size);
    
    return EXIT_SUCCESS;
}