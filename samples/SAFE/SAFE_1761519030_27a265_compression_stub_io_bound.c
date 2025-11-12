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
        
        if (count > 3 || current == 0xFF) {
            output->data[output->size++] = 0xFF;
            output->data[output->size++] = current;
            output->data[output->size++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
                output->data[output->size++] = current;
            }
        }
        
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (!input || !output || input->size == 0) {
        output->size = 0;
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size && output->size < BUFFER_SIZE) {
        if (i + 2 < input->size && input->data[i] == 0xFF) {
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

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    char mode[2];
    Buffer input, output;
    
    printf("Enter input filename: ");
    if (!fgets(input_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    input_filename[strcspn(input_filename, "\n")] = 0;
    
    printf("Enter output filename: ");
    if (!fgets(output_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    output_filename[strcspn(output_filename, "\n")] = 0;
    
    printf("Enter mode (c for compress, d for decompress): ");
    if (!fgets(mode, sizeof(mode), stdin)) {
        fprintf(stderr, "Error reading mode\n");
        return 1;
    }
    
    if (read_file(input_filename, &input) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    if (mode[0] == 'c') {
        simple_compress(&input, &output);
    } else if (mode[0] == 'd') {
        simple_decompress(&input, &output);
    } else {
        fprintf(stderr, "Invalid mode\n");
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