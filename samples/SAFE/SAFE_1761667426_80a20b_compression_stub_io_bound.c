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
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size && out_pos < output_size - 2) {
        uint8_t current = input[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input_size && count < 255 && input[in_pos + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[out_pos++] = 0xFF;
            if (out_pos >= output_size) break;
            output[out_pos++] = current;
            if (out_pos >= output_size) break;
            output[out_pos++] = (uint8_t)count;
            if (out_pos >= output_size) break;
        } else {
            for (size_t i = 0; i < count && out_pos < output_size; i++) {
                output[out_pos++] = current;
            }
        }
        
        in_pos += count;
    }
    
    return out_pos;
}

int compress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return 0;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) return 0;
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return 0;
    }
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE * 2];
    FileHeader header;
    
    memset(&header, 0, sizeof(header));
    strncpy(header.filename, input_filename, MAX_FILENAME - 1);
    header.original_size = 0;
    header.compressed_size = 0;
    
    size_t total_original = 0;
    size_t total_compressed = 0;
    
    size_t bytes_read;
    while ((bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        total_original += bytes_read;
        
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer, sizeof(output_buffer));
        if (compressed_size == 0) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
        
        size_t written = fwrite(output_buffer, 1, compressed_size, output_file);
        if (written != compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
        
        total_compressed += compressed_size;
    }
    
    if (ferror(input_file)) {
        fclose(input_file);
        fclose(output_file);
        return 0;
    }
    
    header.original_size = total_original;
    header.compressed_size = total_compressed;
    
    fseek(output_file, 0, SEEK_SET);
    size_t header_written = fwrite(&header, 1, sizeof(header), output_file);
    if (header_written != sizeof(header)) {
        fclose(input_file);
        fclose(output_file);
        return 0;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    const char* input_filename = argv[1];
    const char* output_filename = argv[2];
    
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    if (strcmp(input_filename, output_filename) == 0) {
        fprintf(stderr, "Input and output files must be different\n");
        return 1;
    }
    
    int result = compress_file(input_filename, output_filename);
    if (!result) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("File compressed successfully\n");
    return 0;
}