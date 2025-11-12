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

int validate_filename(const char* filename) {
    if (!filename || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
        return 0;
    }
    for (size_t i = 0; i < strlen(filename); i++) {
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':') {
            return 0;
        }
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (!input || !output || input_size == 0) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[output_pos++] = 0xFF;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[output_pos++] = current;
            }
        }
        
        if (output_pos > BUFFER_SIZE - 3) {
            break;
        }
        
        i += count;
    }
    
    return output_pos;
}

int process_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return 0;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (!input_file) {
        return 0;
    }
    
    FILE* output_file = fopen(output_filename, "wb");
    if (!output_file) {
        fclose(input_file);
        return 0;
    }
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE];
    file_info_t info;
    
    strncpy(info.filename, input_filename, MAX_FILENAME - 1);
    info.filename[MAX_FILENAME - 1] = '\0';
    info.original_size = 0;
    info.compressed_size = 0;
    
    size_t bytes_read;
    while ((bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        info.original_size += bytes_read;
        
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer);
        if (compressed_size > 0) {
            size_t written = fwrite(output_buffer, 1, compressed_size, output_file);
            if (written != compressed_size) {
                fclose(input_file);
                fclose(output_file);
                return 0;
            }
            info.compressed_size += compressed_size;
        }
    }
    
    if (ferror(input_file)) {
        fclose(input_file);
        fclose(output_file);
        return 0;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    printf("File: %s\n", info.filename);
    printf("Original size: %zu bytes\n", info.original_size);
    printf("Compressed size: %zu bytes\n", info.compressed_size);
    
    if (info.original_size > 0) {
        double ratio = (double)info.compressed_size / info.original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    return 1;
}

int main() {
    char input_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (!fgets(input_file, MAX_FILENAME, stdin)) {
        return 1;
    }
    
    size_t len = strlen(input_file);
    if (len > 0 && input_file[len - 1] == '\n') {
        input_file[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (!fgets(output_file, MAX_FILENAME, stdin)) {
        return 1;
    }
    
    len = strlen(output_file);
    if (len > 0 && output_file[len - 1] == '\n') {
        output_file[len - 1] = '\0';
    }
    
    if (!validate_filename(input_file) || !validate_filename(output_file)) {
        printf("Error: Invalid filename\n");
        return 1;
    }
    
    if (strcmp(input_file, output_file) == 0) {
        printf("Error: Input and output files cannot be the same\n");
        return 1;
    }
    
    if (!process_file(input_file, output_file)) {
        printf("Error: Failed to process file\n");
        return 1;
    }
    
    printf("Compression completed successfully\n");
    return 0;
}