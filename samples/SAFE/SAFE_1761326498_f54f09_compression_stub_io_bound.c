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
    if (ferror(file)) {
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (filename == NULL || buffer == NULL) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    if (written != buffer->size || ferror(file)) {
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) return;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size && out_pos < BUFFER_SIZE - 2) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output->data[out_pos++] = 0xFF;
            output->data[out_pos++] = current;
            output->data[out_pos++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count && out_pos < BUFFER_SIZE; i++) {
                output->data[out_pos++] = current;
            }
        }
        
        in_pos += count;
    }
    
    output->size = out_pos;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) return;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size && out_pos < BUFFER_SIZE) {
        if (in_pos + 2 < input->size && input->data[in_pos] == 0xFF) {
            uint8_t value = input->data[in_pos + 1];
            uint8_t count = input->data[in_pos + 2];
            
            if (out_pos + count > BUFFER_SIZE) break;
            
            for (uint8_t i = 0; i < count; i++) {
                output->data[out_pos++] = value;
            }
            
            in_pos += 3;
        } else {
            output->data[out_pos++] = input->data[in_pos++];
        }
    }
    
    output->size = out_pos;
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
    
    if (output.size == 0) {
        fprintf(stderr, "Processing resulted in empty output\n");
        return 1;
    }
    
    if (write_file(output_file, &output) != 0) {
        fprintf(stderr, "Error writing output file\n");
        return 1;
    }
    
    printf("Success: %s %s -> %s (%zu -> %zu bytes)\n", 
           operation, input_file, output_file, input.size, output.size);
    
    return 0;
}