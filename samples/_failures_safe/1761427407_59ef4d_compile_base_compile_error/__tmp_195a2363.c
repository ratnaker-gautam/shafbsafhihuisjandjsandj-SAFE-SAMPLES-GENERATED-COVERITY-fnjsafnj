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
    if (filename == NULL) return 0;
    size_t len = strlen(filename);
    if (len == 0) return 0;
    if (len >= MAX_FILENAME) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (filename[i] == '/' || filename[i] == '\\' || 
            filename[i] == ':' || filename[i] == '*' || 
            filename[i] == '?' || filename[i] == '"' || 
            filename[i] == '<' || filename[i] == '>' || 
            filename[i] == '|') {
            return 0;
        }
    }
    return 1;
}

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        if (out_idx >= BUFFER_SIZE - 2) return 0;
        
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            for (size_t i = 0; i < count; i++) {
                if (out_idx >= BUFFER_SIZE) return 0;
                output[out_idx++] = input[in_idx++];
            }
        }
    }
    
    *output_size = out_idx;
    return 1;
}

int process_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return 0;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) return 0;
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
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
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) break;
        
        info.original_size += bytes_read;
        
        size_t compressed_size;
        if (!compress_data(input_buffer, bytes_read, output_buffer, &compressed_size)) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
        
        info.compressed_size += compressed_size;
        
        if (fwrite(output_buffer, 1, compressed_size, output_file) != compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
    }
    
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
    char input_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_file, MAX_FILENAME, stdin) == NULL) {
        printf("Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_file);
    if (len > 0 && input_file[len - 1] == '\n') {
        input_file[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_file, MAX_FILENAME, stdin) == NULL) {
        printf("Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_file);
    if (len > 0 && output_file[len - 1] == '\n') {
        output_file[len - 1] = '\0';
    }
    
    if (!validate_filename(input_file) || !validate_filename(output_file)) {
        printf("Invalid filename\n");
        return 1;
    }
    
    if (strcmp(input_file, output_file) == 0) {
        printf("Input and output files cannot be the same\n");
        return 1;
    }
    
    printf("Compressing %s to %s...\n", input_file, output_file);
    
    if (process_file