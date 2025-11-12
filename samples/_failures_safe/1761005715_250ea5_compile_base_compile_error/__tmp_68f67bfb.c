//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: compression_stub
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
        if (total_read > BUFFER_SIZE) total_read = BUFFER_SIZE;
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
        
        if (count >= 3 && output->size < BUFFER_SIZE - 2) {
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

void simple_decompress(const Buffer* input, Buffer* output) {
    if (!input || !output) return;
    
    output->size = 0;
    
    for (size_t i = 0; i < input->size && output->size < BUFFER_SIZE; ) {
        if (i + 2 < input->size && 
            input->data[i] == input->data[i + 1] && 
            output->size + input->data[i + 2] <= BUFFER_SIZE) {
            
            uint8_t value = input->data[i];
            uint8_t count = input->data[i + 2];
            
            for (uint8_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
                output->data[output->size++] = value;
            }
            
            i += 3;
        } else {
            output->data[output->size++] = input->data[i];
            i++;
        }
    }
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    Buffer input_buffer, compressed_buffer, decompressed_buffer;
    
    printf("Enter input filename: ");
    if (!fgets(input_filename, MAX_FILENAME, stdin)) {
        printf("Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (!fgets(output_filename, MAX_FILENAME, stdin)) {
        printf("Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (read_file(input_filename, &input_buffer) != 0) {
        printf("Error reading input file\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buffer.size);
    
    simple_compress(&input_buffer, &compressed_buffer);
    printf("Compressed size: %zu bytes\n", compressed_buffer.size);
    
    if (write_file(output_filename, &compressed_buffer) != 0) {
        printf("Error writing compressed file\n");
        return 1;
    }
    
    printf("Compression complete\n");
    
    simple_decompress(&compressed_buffer, &decompressed_buffer);
    
    if (decompressed_buffer.size == input_buffer.size && 
        memcmp(decompressed_buffer.data, input_buffer.data, input_buffer.size) == 0) {
        printf("Decompression verified: data matches original\n");
    } else {
        printf("