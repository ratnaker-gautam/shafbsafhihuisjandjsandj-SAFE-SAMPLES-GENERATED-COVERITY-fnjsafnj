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
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':' || 
            filename[i] == '*' || filename[i] == '?' || filename[i] == '"' || 
            filename[i] == '<' || filename[i] == '>' || filename[i] == '|') {
            return 0;
        }
    }
    return 1;
}

int compress_file(const char* input_filename, const char* output_filename) {
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
    
    FileHeader header;
    strncpy(header.filename, input_filename, MAX_FILENAME - 1);
    header.filename[MAX_FILENAME - 1] = '\0';
    header.original_size = 0;
    header.compressed_size = 0;
    
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    
    if (file_size < 0 || file_size > INT_MAX) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }
    
    header.original_size = (size_t)file_size;
    
    uint8_t buffer[BUFFER_SIZE];
    size_t total_read = 0;
    size_t total_written = 0;
    
    while (total_read < header.original_size) {
        size_t to_read = BUFFER_SIZE;
        if (header.original_size - total_read < to_read) {
            to_read = header.original_size - total_read;
        }
        
        size_t bytes_read = fread(buffer, 1, to_read, input_file);
        if (bytes_read != to_read && !feof(input_file)) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        
        total_read += bytes_read;
        
        uint8_t compressed[BUFFER_SIZE * 2];
        size_t compressed_size = 0;
        
        for (size_t i = 0; i < bytes_read; i++) {
            if (compressed_size >= BUFFER_SIZE * 2 - 2) {
                break;
            }
            
            uint8_t current = buffer[i];
            uint8_t count = 1;
            
            while (i + 1 < bytes_read && buffer[i + 1] == current && count < 255) {
                count++;
                i++;
            }
            
            compressed[compressed_size++] = count;
            compressed[compressed_size++] = current;
        }
        
        size_t bytes_written = fwrite(compressed, 1, compressed_size, output_file);
        if (bytes_written != compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        
        total_written += compressed_size;
    }
    
    header.compressed_size = total_written;
    
    fseek(output_file, 0, SEEK_SET);
    if (fwrite(&header, sizeof(FileHeader), 1, output_file) != 1) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    return 0;
}

int decompress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return -1;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return -1;
    }
    
    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, input_file) != 1) {
        fclose(input_file);
        return -1;
    }
    
    if (!validate_filename(header.filename)) {
        fclose(input_file);
        return -1;
    }
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return -1;
    }
    
    uint8_t buffer[BUFFER_SIZE];
    size_t total_written = 0;
    
    while (total_written < header.original_size) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0 && !feof(input_file)) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        
        for (size_t i = 0; i < bytes_read; i += 2) {
            if (i + 1 >= bytes_read