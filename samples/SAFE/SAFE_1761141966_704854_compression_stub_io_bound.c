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
            if (i < BUFFER_SIZE - 1 && input_buffer[i] == input_buffer[i + 1]) {
                uint8_t count = 1;
                while (i + count < bytes_read && count < UINT8_MAX && 
                       input_buffer[i] == input_buffer[i + count]) {
                    count++;
                }
                
                if (header.compressed_size + 2 > BUFFER_SIZE) {
                    size_t written = fwrite(output_buffer, 1, header.compressed_size, output_file);
                    if (written != header.compressed_size) {
                        fclose(input_file);
                        fclose(output_file);
                        return -1;
                    }
                    header.compressed_size = 0;
                }
                
                output_buffer[header.compressed_size++] = count;
                output_buffer[header.compressed_size++] = input_buffer[i];
                i += count - 1;
            } else {
                if (header.compressed_size >= BUFFER_SIZE) {
                    size_t written = fwrite(output_buffer, 1, header.compressed_size, output_file);
                    if (written != header.compressed_size) {
                        fclose(input_file);
                        fclose(output_file);
                        return -1;
                    }
                    header.compressed_size = 0;
                }
                output_buffer[header.compressed_size++] = input_buffer[i];
            }
        }
    }
    
    if (header.compressed_size > 0) {
        size_t written = fwrite(output_buffer, 1, header.compressed_size, output_file);
        if (written != header.compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
    }
    
    fseek(output_file, 0, SEEK_SET);
    fwrite(&header, sizeof(FileHeader), 1, output_file);
    
    fclose(input_file);
    fclose(output_file);
    
    return 0;
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_filename, MAX_FILENAME, stdin) == NULL) {
        printf("Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_filename, MAX_FILENAME, stdin) == NULL) {
        printf("Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        printf("Invalid filename\n");
        return 1;
    }
    
    printf("Compressing file...\n");
    
    int result = compress_file(input_filename, output_filename);
    if (result == 0) {
        printf("Compression completed successfully\n");
    } else {
        printf("Compression failed\n");
        return 1;
    }
    
    return 0;
}