//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAX_FILENAME 256

typedef struct {
    char filename[MAX_FILENAME];
    size_t original_size;
    size_t compressed_size;
} file_info_t;

int validate_filename(const char *filename) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
        return 0;
    }
    
    for (size_t i = 0; i < strlen(filename); i++) {
        if (!isalnum((unsigned char)filename[i]) && filename[i] != '.' && 
            filename[i] != '-' && filename[i] != '_') {
            return 0;
        }
    }
    return 1;
}

size_t simple_compress(const char *input, size_t input_len, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len && output_len < output_size - 2) {
        char current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == '\\') {
            if (output_len + 3 >= output_size) break;
            output[output_len++] = '\\';
            output[output_len++] = (char)count;
            output[output_len++] = current;
        } else {
            if (output_len + count >= output_size) break;
            for (size_t j = 0; j < count; j++) {
                output[output_len++] = current;
            }
        }
        
        i += count;
    }
    
    return output_len;
}

int process_file(const char *input_filename, const char *output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return 0;
    }
    
    FILE *input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return 0;
    }
    
    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return 0;
    }
    
    char input_buffer[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE * 2];
    size_t total_input = 0;
    size_t total_output = 0;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        total_input += bytes_read;
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer, sizeof(output_buffer));
        
        if (compressed_size > 0) {
            size_t written = fwrite(output_buffer, 1, compressed_size, output_file);
            if (written != compressed_size) {
                fclose(input_file);
                fclose(output_file);
                return 0;
            }
            total_output += compressed_size;
        }
    }
    
    fclose(input_file);
    fclose(output_file);
    
    printf("File: %s\n", input_filename);
    printf("Original size: %zu bytes\n", total_input);
    printf("Compressed size: %zu bytes\n", total_output);
    printf("Compression ratio: %.2f%%\n", total_input > 0 ? (double)total_output / total_input * 100 : 0.0);
    
    return 1;
}

int main() {
    char input_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_file, sizeof(input_file), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input_file);
    if (len > 0 && input_file[len - 1] == '\n') {
        input_file[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_file, sizeof(output_file), stdin) == NULL) {
        return 1;
    }
    
    len = strlen(output_file);
    if (len > 0 && output_file[len - 1] == '\n') {
        output_file[len - 1] = '\0';
    }
    
    if (!process_file(input_file, output_file)) {
        printf("Error: Failed to process files\n");
        return 1;
    }
    
    return 0;
}