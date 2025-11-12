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
            if (output_pos + 3 > BUFFER_SIZE) {
                break;
            }
            output[output_pos++] = 0xFF;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
        } else {
            if (output_pos + count > BUFFER_SIZE) {
                break;
            }
            for (size_t j = 0; j < count; j++) {
                output[output_pos++] = current;
            }
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
    size_t total_original = 0;
    size_t total_compressed = 0;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer);
        if (compressed_size == 0) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
        
        size_t bytes_written = fwrite(output_buffer, 1, compressed_size, output_file);
        if (bytes_written != compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
        
        total_original += bytes_read;
        total_compressed += compressed_size;
    }
    
    file_info_t info;
    strncpy(info.filename, input_filename, MAX_FILENAME - 1);
    info.filename[MAX_FILENAME - 1] = '\0';
    info.original_size = total_original;
    info.compressed_size = total_compressed;
    
    if (fwrite(&info, sizeof(file_info_t), 1, output_file) != 1) {
        fclose(input_file);
        fclose(output_file);
        return 0;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    return 1;
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (!fgets(input_filename, MAX_FILENAME, stdin)) {
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (!fgets(output_filename, MAX_FILENAME, stdin)) {
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
    
    if (strcmp(input_filename, output_filename) == 0) {
        printf("Input and output filenames cannot be the same\n");
        return 1;
    }
    
    if (!process_file(input_filename, output_filename)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compression completed successfully\n");
    
    FILE* result_file = fopen(output_filename, "rb");
    if (!result_file) {
        return 1;
    }
    
    if (fseek(result_file, -((long)sizeof(file_info_t)), SEEK_END) != 0) {
        fclose(result_file);