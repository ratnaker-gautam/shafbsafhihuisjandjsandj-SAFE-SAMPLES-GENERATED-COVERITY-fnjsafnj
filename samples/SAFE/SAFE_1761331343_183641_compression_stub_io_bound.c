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
    fclose(file);
    
    return buffer->size > 0 ? 0 : -1;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (!filename || !buffer || buffer->size == 0) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    fclose(file);
    
    return written == buffer->size ? 0 : -1;
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
               count < 255) {
            count++;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (!input || !output || input->size == 0) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i + 1 < input->size && output->size < BUFFER_SIZE) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        for (uint8_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
            output->data[output->size++] = value;
        }
        
        i += 2;
    }
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    Buffer input_buffer, compressed_buffer, decompressed_buffer;
    
    printf("Enter input filename: ");
    if (!fgets(input_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    if (read_file(input_filename, &input_buffer) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    printf("Original file size: %zu bytes\n", input_buffer.size);
    
    simple_compress(&input_buffer, &compressed_buffer);
    printf("Compressed size: %zu bytes\n", compressed_buffer.size);
    
    printf("Enter compressed output filename: ");
    if (!fgets(output_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (write_file(output_filename, &compressed_buffer) != 0) {
        fprintf(stderr, "Error writing compressed file\n");
        return 1;
    }
    
    simple_decompress(&compressed_buffer, &decompressed_buffer);
    
    if (decompressed_buffer.size != input_buffer.size || 
        memcmp(decompressed_buffer.data, input_buffer.data, input_buffer.size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed_buffer.size / input_buffer.size * 100.0);
    
    return 0;
}