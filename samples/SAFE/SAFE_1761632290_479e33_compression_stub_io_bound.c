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
    int close_result = fclose(file);
    
    if (buffer->size == 0 || close_result != 0) return -1;
    return 0;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (filename == NULL || buffer == NULL || buffer->size == 0) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    int close_result = fclose(file);
    
    if (written != buffer->size || close_result != 0) return -1;
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
               count < 255 && 
               input->data[in_pos + count] == current) {
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

int main(void) {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_filename, MAX_FILENAME, stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    if (strlen(input_filename) == 0) {
        fprintf(stderr, "Invalid input filename\n");
        return 1;
    }
    
    printf("Enter output filename: ");
    if (fgets(output_filename, MAX_FILENAME, stdin) == NULL) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (strlen(output_filename) == 0) {
        fprintf(stderr, "Invalid output filename\n");
        return 1;
    }
    
    Buffer input_data = {0};
    Buffer output_data = {0};
    
    if (read_file(input_filename, &input_data) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    simple_compress(&input_data, &output_data);
    
    if (write_file(output_filename, &output_data) != 0) {
        fprintf(stderr, "Error writing output file\n");
        return 1;
    }
    
    printf("Compression complete. Input: %zu bytes, Output: %zu bytes\n", 
           input_data.size, output_data.size);
    
    return 0;
}