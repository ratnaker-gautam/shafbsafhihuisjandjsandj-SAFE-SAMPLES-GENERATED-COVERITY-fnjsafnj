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
    if (ferror(file)) {
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (!filename || !buffer || buffer->size > BUFFER_SIZE) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    if (written != buffer->size || ferror(file)) {
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
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
        
        if (count > 3 || current == 0xFF) {
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count && out_idx < BUFFER_SIZE; i++) {
                output->data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output->size = out_idx;
}

int validate_filename(const char* filename) {
    if (!filename || strlen(filename) >= MAX_FILENAME) return 0;
    
    for (size_t i = 0; i < strlen(filename); i++) {
        if (filename[i] == '/' || filename[i] == '\\' || 
            filename[i] == ':' || filename[i] == '*' || 
            filename[i] == '?' || filename[i] == '"' || 
            filename[i] == '<' || filename[i] == '>' || 
            filename[i] == '|') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (!fgets(input_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    if (!validate_filename(input_filename)) {
        fprintf(stderr, "Invalid input filename\n");
        return 1;
    }
    
    printf("Enter output filename: ");
    if (!fgets(output_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (!validate_filename(output_filename)) {
        fprintf(stderr, "Invalid output filename\n");
        return 1;
    }
    
    if (strcmp(input_filename, output_filename) == 0) {
        fprintf(stderr, "Input and output filenames must be different\n");
        return 1;
    }
    
    Buffer input, output;
    
    if (read_file(input_filename, &input) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Input file is empty\n");
        return 1;
    }
    
    simple_compress(&input, &output);
    
    if (write_file(output_filename, &output) != 0) {
        fprintf(stderr, "Error writing output file\n");
        return 1;
    }
    
    printf("Compression complete. Input: %zu bytes, Output: %zu bytes\n", 
           input.size, output.size);
    
    return 0;
}