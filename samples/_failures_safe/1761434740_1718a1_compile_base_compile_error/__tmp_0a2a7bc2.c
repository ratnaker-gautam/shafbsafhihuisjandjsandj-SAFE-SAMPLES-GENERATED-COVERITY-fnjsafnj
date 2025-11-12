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
    if (!input || !output || input->size == 0) {
        output->size = 0;
        return;
    }
    
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
            if (out_pos + 3 > BUFFER_SIZE) break;
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
    if (!input || !output || input->size == 0) {
        output->size = 0;
        return;
    }
    
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

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    Buffer input, compressed, decompressed;
    
    printf("Enter input filename: ");
    if (!fgets(input_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    if (read_file(input_filename, &input) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    printf("Enter compressed output filename: ");
    if (!fgets(output_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    simple_compress(&input, &compressed);
    
    if (write_file(output_filename, &compressed) != 0) {
        fprintf(stderr, "Error writing compressed file\n");
        return 1;
    }
    
    printf("Compression complete. Original size: %zu, Compressed size: %zu\n", 
           input.size, compressed.size);
    
    printf("Enter decompressed output filename: ");
    if (!fgets(output_filename, MAX_FILENAME, stdin)) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    simple_decompress(&compressed, &decompressed);
    
    if (write_file(output_filename, &decompressed) != 0) {
        fprintf(stderr, "Error writing decompressed