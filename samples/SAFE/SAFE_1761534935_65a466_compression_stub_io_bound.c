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
    size_t total_read = 0;
    size_t total_written = 0;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        if (total_read > SIZE_MAX - bytes_read) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        total_read += bytes_read;
        
        size_t output_pos = 0;
        for (size_t i = 0; i < bytes_read; i++) {
            if (output_pos >= BUFFER_SIZE) {
                break;
            }
            output_buffer[output_pos++] = input_buffer[i];
            if (i + 1 < bytes_read && input_buffer[i] == input_buffer[i + 1]) {
                uint8_t count = 1;
                while (i + count < bytes_read && input_buffer[i] == input_buffer[i + count] && count < UINT8_MAX) {
                    count++;
                }
                output_buffer[output_pos++] = count;
                i += count - 1;
            }
        }
        
        size_t bytes_written = fwrite(output_buffer, 1, output_pos, output_file);
        if (bytes_written != output_pos) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        
        if (total_written > SIZE_MAX - bytes_written) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        total_written += bytes_written;
    }
    
    header.original_size = total_read;
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
    
    if (strcmp(input_filename, output_filename) == 0) {
        printf("Input and output filenames cannot be the same\n");
        return 1;
    }
    
    int result = compress_file(input_filename, output_filename);
    if (result == 0) {
        printf("File compressed successfully\n");
    } else {
        printf("Error compressing file\n");
        return 1;
    }
    
    return 0;
}