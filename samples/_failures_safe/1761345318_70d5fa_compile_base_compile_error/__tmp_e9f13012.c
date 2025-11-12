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
        if (ferror(file)) {
            fclose(file);
            return -1;
        }
        
        total_read += bytes_read;
        if (bytes_read == 0) break;
    }
    
    buffer->size = total_read;
    fclose(file);
    return 0;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (!filename || !buffer) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    
    size_t total_written = 0;
    while (total_written < buffer->size) {
        size_t to_write = buffer->size - total_written;
        if (to_write > BUFFER_SIZE) to_write = BUFFER_SIZE;
        
        size_t bytes_written = fwrite(buffer->data + total_written, 1, to_write, file);
        if (bytes_written == 0 || ferror(file)) {
            fclose(file);
            return -1;
        }
        
        total_written += bytes_written;
    }
    
    fclose(file);
    return 0;
}

void simple_compress(Buffer* input, Buffer* output) {
    if (!input || !output || input->size == 0) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size && output->size < BUFFER_SIZE - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && 
               input->data[i + count] == current && 
               count < 255) {
            count++;
        }
        
        if (output->size + 2 <= BUFFER_SIZE) {
            output->data[output->size++] = (uint8_t)count;
            output->data[output->size++] = current;
        }
        
        i += count;
    }
}

void simple_decompress(Buffer* input, Buffer* output) {
    if (!input || !output || input->size == 0) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i + 1 < input->size && output->size < BUFFER_SIZE) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        size_t bytes_to_write = (size_t)count;
        if (bytes_to_write > BUFFER_SIZE - output->size) {
            bytes_to_write = BUFFER_SIZE - output->size;
        }
        
        for (size_t j = 0; j < bytes_to_write; j++) {
            output->data[output->size++] = value;
        }
        
        i += 2;
    }
}

int main(void) {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    Buffer input_buffer, compressed_buffer, decompressed_buffer;
    
    printf("Enter input filename: ");
    if (!fgets(input_filename, sizeof(input_filename), stdin)) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
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
    
    if (read_file(input_filename, &input_buffer) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buffer.size);
    
    simple_compress(&input_buffer, &compressed_buffer);
    printf("Compressed size: %zu bytes\n", compressed_buffer.size);
    
    if (write_file(output_filename, &compressed_buffer) != 0) {
        fprintf(stderr, "Error writing compressed file\n");
        return 1;
    }
    
    printf("File compressed successfully\n");
    
    simple_decompress(&compressed_buffer, &decompressed_buffer);
    
    if (decompressed_buffer.size == input_buffer.size && 
        memcmp(decompressed_buffer.data, input_buffer.data, input_buffer.size) == 0) {
        printf