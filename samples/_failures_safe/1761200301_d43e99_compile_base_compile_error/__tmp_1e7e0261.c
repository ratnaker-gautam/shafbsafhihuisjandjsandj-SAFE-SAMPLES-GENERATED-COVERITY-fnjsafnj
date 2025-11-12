//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 4096
#define MAX_FILENAME 256

typedef struct {
    char filename[MAX_FILENAME];
    size_t original_size;
    size_t compressed_size;
} FileHeader;

int validate_filename(const char* filename) {
    if (filename == NULL) return 0;
    if (strlen(filename) == 0) return 0;
    if (strlen(filename) >= MAX_FILENAME) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return 0;
    if (output_size < input_size) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size && out_pos < output_size - 2) {
        uint8_t current = input[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input_size && count < 255 && input[in_pos + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output[out_pos++] = 0xFF;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
            in_pos += count;
        } else {
            for (size_t i = 0; i < count && out_pos < output_size; i++) {
                output[out_pos++] = current;
            }
            in_pos += count;
        }
    }
    
    return out_pos;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size && out_pos < output_size) {
        if (in_pos + 2 < input_size && input[in_pos] == 0xFF) {
            uint8_t value = input[in_pos + 1];
            uint8_t count = input[in_pos + 2];
            
            if (out_pos + count > output_size) return 0;
            
            for (uint8_t i = 0; i < count; i++) {
                output[out_pos++] = value;
            }
            in_pos += 3;
        } else {
            output[out_pos++] = input[in_pos++];
        }
    }
    
    return out_pos;
}

int compress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) return 1;
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) return 1;
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return 1;
    }
    
    FileHeader header;
    memset(&header, 0, sizeof(header));
    strncpy(header.filename, input_filename, MAX_FILENAME - 1);
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE * 2];
    size_t total_original = 0;
    size_t total_compressed = 0;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) break;
        
        total_original += bytes_read;
        
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer, sizeof(output_buffer));
        if (compressed_size == 0) {
            fclose(input_file);
            fclose(output_file);
            return 1;
        }
        
        size_t bytes_written = fwrite(output_buffer, 1, compressed_size, output_file);
        if (bytes_written != compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return 1;
        }
        
        total_compressed += compressed_size;
    }
    
    header.original_size = total_original;
    header.compressed_size = total_compressed;
    
    fseek(output_file, 0, SEEK_SET);
    if (fwrite(&header, sizeof(header), 1, output_file) != 1) {
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    
    fclose(input_file);
    fclose(output_file);
    return 0;
}

int decompress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) return 1;
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) return 1;
    
    FileHeader header;
    if (fread(&header, sizeof(header), 1, input_file) != 1) {
        fclose(input_file);
        return 1;
    }
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return 1;
    }