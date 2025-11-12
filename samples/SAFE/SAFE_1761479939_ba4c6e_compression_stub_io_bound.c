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

size_t simulate_compression(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return 0;
    
    size_t compressed = 0;
    size_t i = 0;
    
    while (i < size) {
        uint8_t current = data[i];
        size_t count = 1;
        
        while (i + count < size && data[i + count] == current && count < 255) {
            count++;
        }
        
        compressed += 2;
        i += count;
    }
    
    return (compressed < size) ? compressed : size;
}

int process_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return 0;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return 0;
    }
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return 0;
    }
    
    uint8_t buffer[BUFFER_SIZE];
    size_t total_read = 0;
    size_t total_written = 0;
    
    while (1) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        total_read += bytes_read;
        
        for (size_t i = 0; i < bytes_read; i++) {
            uint8_t current = buffer[i];
            size_t count = 1;
            
            while (i + count < bytes_read && buffer[i + count] == current && count < 255) {
                count++;
            }
            
            uint8_t header[2] = {current, (uint8_t)count};
            size_t written = fwrite(header, 1, 2, output_file);
            if (written != 2) {
                fclose(input_file);
                fclose(output_file);
                return 0;
            }
            total_written += 2;
            i += count - 1;
        }
    }
    
    fclose(input_file);
    fclose(output_file);
    
    file_info_t info;
    strncpy(info.filename, input_filename, MAX_FILENAME - 1);
    info.filename[MAX_FILENAME - 1] = '\0';
    info.original_size = total_read;
    info.compressed_size = total_written;
    
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
    if (fgets(input_file, MAX_FILENAME, stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input_file);
    if (len > 0 && input_file[len - 1] == '\n') {
        input_file[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_file, MAX_FILENAME, stdin) == NULL) {
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