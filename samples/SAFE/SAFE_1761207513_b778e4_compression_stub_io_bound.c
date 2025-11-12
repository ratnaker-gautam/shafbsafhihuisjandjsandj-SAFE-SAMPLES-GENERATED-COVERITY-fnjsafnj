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
} file_info_t;

int validate_filename(const char *filename) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
        return 0;
    }
    
    for (size_t i = 0; i < strlen(filename); i++) {
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':' || 
            filename[i] == '*' || filename[i] == '?' || filename[i] == '"' || 
            filename[i] == '<' || filename[i] == '>' || filename[i] == '|') {
            return 0;
        }
    }
    
    return 1;
}

int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0) {
        return 0;
    }
    
    if (input_size > BUFFER_SIZE || *output_size < input_size) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 > *output_size) {
                return 0;
            }
            output[out_idx++] = 0xFF;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
        } else {
            if (out_idx + count > *output_size) {
                return 0;
            }
            for (size_t i = 0; i < count; i++) {
                output[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    *output_size = out_idx;
    return 1;
}

int process_file(const char *input_filename, const char *output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return 0;
    }
    
    FILE *input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return 0;
    }
    
    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return 0;
    }
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE];
    file_info_t file_info;
    
    strncpy(file_info.filename, input_filename, MAX_FILENAME - 1);
    file_info.filename[MAX_FILENAME - 1] = '\0';
    file_info.original_size = 0;
    file_info.compressed_size = 0;
    
    size_t total_compressed = 0;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        file_info.original_size += bytes_read;
        
        size_t output_size = BUFFER_SIZE;
        if (!compress_data(input_buffer, bytes_read, output_buffer, &output_size)) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
        
        size_t bytes_written = fwrite(output_buffer, 1, output_size, output_file);
        if (bytes_written != output_size) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
        
        total_compressed += output_size;
    }
    
    file_info.compressed_size = total_compressed;
    
    if (ferror(input_file) || ferror(output_file)) {
        fclose(input_file);
        fclose(output_file);
        return 0;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    printf("File: %s\n", file_info.filename);
    printf("Original size: %zu bytes\n", file_info.original_size);
    printf("Compressed size: %zu bytes\n", file_info.compressed_size);
    
    if (file_info.original_size > 0) {
        double ratio = (double)file_info.compressed_size / file_info.original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    if (!process_file(argv[1], argv[2])) {
        printf("Error: Failed to process file\n");
        return 1;
    }
    
    printf("Compression completed successfully\n");
    return 0;
}