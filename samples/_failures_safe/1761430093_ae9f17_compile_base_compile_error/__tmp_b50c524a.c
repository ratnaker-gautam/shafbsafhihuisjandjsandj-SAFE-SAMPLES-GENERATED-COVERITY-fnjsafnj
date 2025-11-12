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
    
    buffer->size = 0;
    size_t total_read = 0;
    
    while (!feof(file)) {
        if (total_read >= BUFFER_SIZE) break;
        
        size_t to_read = BUFFER_SIZE - total_read;
        if (to_read > BUFFER_SIZE) to_read = BUFFER_SIZE;
        
        size_t bytes_read = fread(buffer->data + total_read, 1, to_read, file);
        if (bytes_read == 0) break;
        
        total_read += bytes_read;
        if (total_read > BUFFER_SIZE) {
            fclose(file);
            return -1;
        }
    }
    
    buffer->size = total_read;
    fclose(file);
    return 0;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (!filename || !buffer || buffer->size > BUFFER_SIZE) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    fclose(file);
    
    return (written == buffer->size) ? 0 : -1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (!input || !output || input->size > BUFFER_SIZE) return;
    
    output->size = 0;
    
    for (size_t i = 0; i < input->size && output->size < BUFFER_SIZE - 2; ) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && 
               input->data[i + count] == current && 
               count < 255 && 
               output->size < BUFFER_SIZE - 2) {
            count++;
        }
        
        if (output->size + 2 <= BUFFER_SIZE) {
            output->data[output->size++] = (uint8_t)count;
            output->data[output->size++] = current;
        }
        
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (!input || !output || input->size > BUFFER_SIZE) return;
    
    output->size = 0;
    
    for (size_t i = 0; i + 1 < input->size && output->size < BUFFER_SIZE; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        for (uint8_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
            output->data[output->size++] = value;
        }
    }
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    int choice;
    
    printf("Simple Run-Length Compression/Decompression\n");
    printf("Enter input filename: ");
    
    if (!fgets(input_filename, sizeof(input_filename), stdin)) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    if (strlen(input_filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    printf("Enter output filename: ");
    
    if (!fgets(output_filename, sizeof(output_filename), stdin)) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (strlen(output_filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Compress\n");
    printf("2. Decompress\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    Buffer input_buffer, output_buffer;
    
    if (read_file(input_filename, &input_buffer) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    if (input_buffer.size == 0) {
        fprintf(stderr, "Input file is empty\n");
        return 1;
    }
    
    if (choice == 1) {
        simple_compress(&input_buffer, &output_buffer);
        printf("Compressed %zu bytes to %zu bytes\n", input_buffer.size, output_buffer.size);
    } else if (choice == 2) {
        simple_de