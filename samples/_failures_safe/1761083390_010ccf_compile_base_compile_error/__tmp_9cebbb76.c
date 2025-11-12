//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: compression_stub
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

int validate_filename(const char *filename) {
    if (filename == NULL) {
        return 0;
    }
    
    size_t filename_len = strlen(filename);
    if (filename_len == 0 || filename_len >= MAX_FILENAME) {
        return 0;
    }
    
    for (size_t i = 0; i < filename_len; i++) {
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':' || 
            filename[i] == '*' || filename[i] == '?' || filename[i] == '"' || 
            filename[i] == '<' || filename[i] == '>' || filename[i] == '|') {
            return 0;
        }
    }
    
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    if (output_size < input_size) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size && output_idx < output_size - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3) {
            if (output_idx + 3 > output_size) {
                return 0;
            }
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && output_idx < output_size; j++) {
                output[output_idx++] = current;
            }
            i += count;
        }
    }
    
    if (i < input_size) {
        return 0;
    }
    
    return output_idx;
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
    
    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE * 2];
    size_t total_input = 0;
    size_t total_output = 0;
    int success = 1;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            if (ferror(input_file)) {
                success = 0;
            }
            break;
        }
        
        if (total_input > SIZE_MAX - bytes_read) {
            success = 0;
            break;
        }
        total_input += bytes_read;
        
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer, sizeof(output_buffer));
        if (compressed_size == 0) {
            success = 0;
            break;
        }
        
        size_t bytes_written = fwrite(output_buffer, 1, compressed_size, output_file);
        if (bytes_written != compressed_size) {
            success = 0;
            break;
        }
        
        if (total_output > SIZE_MAX - compressed_size) {
            success = 0;
            break;
        }
        total_output += compressed_size;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    if (success) {
        printf("Compression completed:\n");
        printf("Input file: %s (%zu bytes)\n", input_filename, total_input);
        printf("Output file: %s (%zu bytes)\n", output_filename, total_output);
        if (total_input > 0) {
            double ratio = (100.0 * (double)total_output) / (double)total_input;
            printf("Compression ratio: %.2f%%\n", ratio);
        } else {
            printf("Compression ratio: 0.00%%\n");
        }
        return 1;
    } else {
        return 0;
    }
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Simple File Compression Tool\n");
    printf("Enter input filename: ");
    
    if (fgets(input_filename, sizeof(input_filename), stdin) == NULL) {
        printf("Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    
    if (fgets(output_filename, sizeof