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
    
    buffer->size = fread(buffer->data, 1, BUFFER_SIZE, file);
    int result = ferror(file) ? 0 : 1;
    fclose(file);
    return result;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (!filename || !buffer || buffer->size > BUFFER_SIZE) return 0;
    
    FILE* file = fopen(filename, "wb");
    if (!file) return 0;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    int result = (written == buffer->size && !ferror(file)) ? 1 : 0;
    fclose(file);
    return result;
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
            if (out_idx + 2 >= BUFFER_SIZE) break;
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

void simple_decompress(const Buffer* input, Buffer* output) {
    if (!input || !output) {
        output->size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < BUFFER_SIZE) {
        if (in_idx + 2 < input->size && input->data[in_idx] == 0xFF) {
            uint8_t value = input->data[in_idx + 1];
            uint8_t count = input->data[in_idx + 2];
            
            if (out_idx + count > BUFFER_SIZE) break;
            
            for (uint8_t i = 0; i < count; i++) {
                output->data[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output->data[out_idx++] = input->data[in_idx++];
        }
    }
    
    output->size = out_idx;
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
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
    
    if (!read_file(input_filename, &input_buffer)) {
        printf("Error reading input file\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buffer.size);
    
    simple_compress(&input_buffer, &compressed_buffer);
    printf("Compressed size: %zu bytes\n", compressed_buffer.size);
    
    if (!write_file(output_filename, &compressed_buffer)) {
        printf("Error writing compressed file\n");
        return 1;
    }
    
    simple_decompress(&compressed_buffer, &decompressed_buffer);
    
    if (decompressed_buffer.size == input_buffer.size && 
        memcmp(decompressed_buffer.data, input_buffer.data, input_buffer.size) == 0) {
        printf("Decompression verified successfully\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("Compression completed successfully\n");
    return 0;
}