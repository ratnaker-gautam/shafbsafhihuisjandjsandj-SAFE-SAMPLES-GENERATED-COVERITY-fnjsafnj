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
    size_t bytes_read;
    
    while ((bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        header.original_size += bytes_read;
        
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
        header.compressed_size += bytes_written;
    }
    
    if (ferror(input_file)) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }
    
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
    size_t total_bytes = 0;
    size_t bytes_read;
    
    while ((bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            if (i < BUFFER_SIZE) {
                output_buffer[i] = input_buffer[i] ^ 0xAA;
            }
        }
        
        size_t bytes_to_write = bytes_read;
        if (total_bytes + bytes_to_write > header.original_size) {
            bytes_to_write = header.original_size - total_bytes;
        }
        
        if (fwrite(output_buffer, 1, bytes_to_write, output_file) != bytes_to_write) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        
        total_bytes += bytes_to_write;
        if (total_bytes >= header.original_size) {
            break;
        }
    }
    
    if (ferror(input_file) || total_bytes != header.original_size) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }
    
    fclose(input_file);
    fclose(output_file);
    return 0;
}

int main() {
    char input_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    int choice;
    
    printf("File Compression Tool\n");
    printf("1. Compress file\n");
    printf("2. Decompress file\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1)