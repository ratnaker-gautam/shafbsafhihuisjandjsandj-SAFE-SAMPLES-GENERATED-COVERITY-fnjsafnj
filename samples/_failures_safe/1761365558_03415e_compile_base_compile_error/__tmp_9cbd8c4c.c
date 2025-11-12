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
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE];
    size_t total_input = 0;
    size_t total_output = 0;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        total_input += bytes_read;
        
        for (size_t i = 0; i < bytes_read; i++) {
            if (i < BUFFER_SIZE) {
                output_buffer[i] = input_buffer[i] ^ 0xAA;
            }
        }
        
        size_t bytes_written = fwrite(output_buffer, 1, bytes_read, output_file);
        if (bytes_written != bytes_read) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        total_output += bytes_written;
    }
    
    if (ferror(input_file)) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }
    
    header.original_size = total_input;
    header.compressed_size = total_output;
    
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
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE];
    size_t total_processed = 0;
    
    while (total_processed < header.compressed_size) {
        size_t bytes_to_read = BUFFER_SIZE;
        if (header.compressed_size - total_processed < BUFFER_SIZE) {
            bytes_to_read = header.compressed_size - total_processed;
        }
        
        size_t bytes_read = fread(input_buffer, 1, bytes_to_read, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        for (size_t i = 0; i < bytes_read; i++) {
            if (i < BUFFER_SIZE) {
                output_buffer[i] = input_buffer[i] ^ 0xAA;
            }
        }
        
        size_t bytes_written = fwrite(output_buffer, 1, bytes_read, output_file);
        if (bytes_written != bytes_read) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        total_processed += bytes_read;
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

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    int choice;
    
    printf("File Compression Tool