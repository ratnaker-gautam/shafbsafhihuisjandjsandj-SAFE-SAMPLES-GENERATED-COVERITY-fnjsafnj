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

int compress_file(const char* input_filename, const char* output_filename, file_info_t* info) {
    if (input_filename == NULL || output_filename == NULL || info == NULL) {
        return -1;
    }
    
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return -1;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return -1;
    }
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return -1;
    }
    
    strncpy(info->filename, input_filename, MAX_FILENAME - 1);
    info->filename[MAX_FILENAME - 1] = '\0';
    info->original_size = 0;
    info->compressed_size = 0;
    
    uint8_t buffer[BUFFER_SIZE];
    uint8_t compressed[BUFFER_SIZE * 2];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        info->original_size += bytes_read;
        
        size_t compressed_size = 0;
        for (size_t i = 0; i < bytes_read; i++) {
            if (i + 1 < bytes_read && buffer[i] == buffer[i + 1]) {
                uint8_t count = 1;
                while (i + count < bytes_read && buffer[i] == buffer[i + count] && count < UINT8_MAX) {
                    count++;
                }
                
                if (compressed_size + 2 > sizeof(compressed)) {
                    fclose(input_file);
                    fclose(output_file);
                    return -1;
                }
                
                compressed[compressed_size++] = buffer[i];
                compressed[compressed_size++] = count;
                i += count - 1;
            } else {
                if (compressed_size + 1 > sizeof(compressed)) {
                    fclose(input_file);
                    fclose(output_file);
                    return -1;
                }
                compressed[compressed_size++] = buffer[i];
            }
        }
        
        if (fwrite(compressed, 1, compressed_size, output_file) != compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        
        info->compressed_size += compressed_size;
    }
    
    if (ferror(input_file)) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }
    
    fclose(input_file);
    fclose(output_file);
    return 0;
}

int main(void) {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_filename, sizeof(input_filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_filename, sizeof(output_filename), stdin) == NULL) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (strcmp(input_filename, output_filename) == 0) {
        fprintf(stderr, "Input and output filenames cannot be the same\n");
        return 1;
    }
    
    file_info_t compression_info;
    
    printf("Compressing file...\n");
    if (compress_file(input_filename, output_filename, &compression_info) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compression completed successfully\n");
    printf("File: %s\n", compression_info.filename);
    printf("Original size: %zu bytes\n", compression_info.original_size);
    printf("Compressed size: %zu bytes\n", compression_info.compressed_size);
    
    if (compression_info.original_size > 0) {
        double ratio = (double)compression_info.compressed_size / compression_info.original_size * 100.0;
        printf("Compression ratio: %.2f%%\n",