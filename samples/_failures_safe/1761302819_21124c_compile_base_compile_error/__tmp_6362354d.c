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
} FileInfo;

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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size && out_pos < output_size - 2) {
        uint8_t current = input[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input_size && count < 255 && input[in_pos + count] == current) {
            count++;
        }
        
        if (count > 3) {
            if (out_pos + 2 >= output_size) break;
            output[out_pos++] = 0xFF;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
            in_pos += count;
        } else {
            if (out_pos + count >= output_size) break;
            for (size_t i = 0; i < count; i++) {
                output[out_pos++] = current;
            }
            in_pos += count;
        }
    }
    
    return out_pos;
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
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE * 2];
    size_t total_input = 0;
    size_t total_output = 0;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        total_input += bytes_read;
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer, sizeof(output_buffer));
        
        if (compressed_size == 0 || fwrite(output_buffer, 1, compressed_size, output_file) != compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return 0;
        }
        
        total_output += compressed_size;
    }
    
    FileInfo info;
    strncpy(info.filename, input_filename, MAX_FILENAME - 1);
    info.filename[MAX_FILENAME - 1] = '\0';
    info.original_size = total_input;
    info.compressed_size = total_output;
    
    if (fwrite(&info, sizeof(FileInfo), 1, output_file) != 1) {
        fclose(input_file);
        fclose(output_file);
        return 0;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    printf("Compression completed: %s -> %s\n", input_filename, output_filename);
    printf("Original size: %zu bytes\n", total_input);
    printf("Compressed size: %zu bytes\n", total_output);
    printf("Compression ratio: %.2f%%\n", total_input > 0 ? (100.0 * total_output / total_input) : 0.0);
    
    return 1;
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_filename, MAX_FILENAME, stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_filename, MAX_FILENAME, stdin) == NULL) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (!process_file(input_filename, output_filename)) {
        fprintf(stderr,