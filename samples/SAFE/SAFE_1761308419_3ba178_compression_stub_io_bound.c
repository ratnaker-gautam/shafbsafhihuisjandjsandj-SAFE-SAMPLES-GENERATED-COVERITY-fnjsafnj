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
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
        return 0;
    }
    for (size_t i = 0; i < strlen(filename); i++) {
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':') {
            return 0;
        }
    }
    return 1;
}

int compress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return -1;
    }
    
    FILE* input = fopen(input_filename, "rb");
    if (input == NULL) {
        return -1;
    }
    
    FILE* output = fopen(output_filename, "wb");
    if (output == NULL) {
        fclose(input);
        return -1;
    }
    
    FileHeader header;
    strncpy(header.filename, input_filename, MAX_FILENAME - 1);
    header.filename[MAX_FILENAME - 1] = '\0';
    header.original_size = 0;
    header.compressed_size = 0;
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input)) > 0) {
        header.original_size += bytes_read;
        
        for (size_t i = 0; i < bytes_read; i++) {
            if (i < BUFFER_SIZE - 1 && input_buffer[i] == input_buffer[i + 1]) {
                uint8_t count = 1;
                while (i + count < bytes_read && count < UINT8_MAX && 
                       input_buffer[i] == input_buffer[i + count]) {
                    count++;
                }
                
                if (header.compressed_size + 2 > BUFFER_SIZE) {
                    size_t written = fwrite(output_buffer, 1, header.compressed_size, output);
                    if (written != header.compressed_size) {
                        fclose(input);
                        fclose(output);
                        return -1;
                    }
                    header.compressed_size = 0;
                }
                
                output_buffer[header.compressed_size++] = count;
                output_buffer[header.compressed_size++] = input_buffer[i];
                i += count - 1;
            } else {
                if (header.compressed_size + 1 > BUFFER_SIZE) {
                    size_t written = fwrite(output_buffer, 1, header.compressed_size, output);
                    if (written != header.compressed_size) {
                        fclose(input);
                        fclose(output);
                        return -1;
                    }
                    header.compressed_size = 0;
                }
                output_buffer[header.compressed_size++] = input_buffer[i];
            }
        }
    }
    
    if (header.compressed_size > 0) {
        size_t written = fwrite(output_buffer, 1, header.compressed_size, output);
        if (written != header.compressed_size) {
            fclose(input);
            fclose(output);
            return -1;
        }
    }
    
    if (ferror(input)) {
        fclose(input);
        fclose(output);
        return -1;
    }
    
    fclose(input);
    fclose(output);
    
    printf("Compression completed: %s -> %s\n", input_filename, output_filename);
    printf("Original size: %zu bytes\n", header.original_size);
    printf("Compressed size: %zu bytes\n", header.compressed_size);
    
    return 0;
}

int main() {
    char input_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_file, MAX_FILENAME, stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_file);
    if (len > 0 && input_file[len - 1] == '\n') {
        input_file[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_file, MAX_FILENAME, stdin) == NULL) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_file);
    if (len > 0 && output_file[len - 1] == '\n') {
        output_file[len - 1] = '\0';
    }
    
    if (strcmp(input_file, output_file) == 0) {
        fprintf(stderr, "Input and output files cannot be the same\n");
        return 1;
    }
    
    int result = compress_file(input_file, output_file);
    if (result != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    return 0;
}