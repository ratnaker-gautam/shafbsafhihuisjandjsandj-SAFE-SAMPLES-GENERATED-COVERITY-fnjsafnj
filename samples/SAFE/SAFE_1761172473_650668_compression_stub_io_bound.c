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
    if (!filename || !buffer) return 0;
    
    FILE* file = fopen(filename, "rb");
    if (!file) return 0;
    
    buffer->size = 0;
    size_t total_read = 0;
    
    while (!feof(file) && total_read < BUFFER_SIZE) {
        size_t bytes_read = fread(buffer->data + total_read, 1, BUFFER_SIZE - total_read, file);
        if (bytes_read == 0) break;
        total_read += bytes_read;
    }
    
    buffer->size = total_read;
    fclose(file);
    return total_read > 0;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (!filename || !buffer || buffer->size == 0) return 0;
    
    FILE* file = fopen(filename, "wb");
    if (!file) return 0;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    fclose(file);
    return written == buffer->size;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (!input || !output || input->size == 0) return;
    
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
        
        if (count >= 3) {
            output->data[output->size++] = current;
            output->data[output->size++] = current;
            output->data[output->size++] = (uint8_t)count;
            i += count;
        } else {
            output->data[output->size++] = current;
            i++;
        }
    }
}

int validate_filename(const char* filename) {
    if (!filename || strlen(filename) >= MAX_FILENAME) return 0;
    
    for (size_t i = 0; filename[i]; i++) {
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
        fprintf(stderr, "Input and output filenames cannot be the same\n");
        return 1;
    }
    
    Buffer input_data;
    Buffer output_data;
    
    if (!read_file(input_filename, &input_data)) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    if (input_data.size == 0) {
        fprintf(stderr, "Input file is empty\n");
        return 1;
    }
    
    simple_compress(&input_data, &output_data);
    
    if (output_data.size == 0) {
        fprintf(stderr, "Compression produced no output\n");
        return 1;
    }
    
    if (!write_file(output_filename, &output_data)) {
        fprintf(stderr, "Error writing output file\n");
        return 1;
    }
    
    printf("Compression complete. Input: %zu bytes, Output: %zu bytes\n", 
           input_data.size, output_data.size);
    
    return 0;
}