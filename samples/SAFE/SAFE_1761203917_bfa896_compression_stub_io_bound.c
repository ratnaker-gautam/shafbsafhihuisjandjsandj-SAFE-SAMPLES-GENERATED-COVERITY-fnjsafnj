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

int validate_filename(const char *filename) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
        return 0;
    }
    for (size_t i = 0; i < strlen(filename); i++) {
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':') {
            return 0;
        }
    }
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size < input_size) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output_pos >= output_size) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3) {
            if (output_pos + 2 > output_size) {
                return 0;
            }
            output[output_pos++] = 0xFF;
            output[output_pos++] = count;
            output[output_pos++] = current;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (output_pos >= output_size) {
                    return 0;
                }
                output[output_pos++] = current;
            }
            i += count;
        }
    }
    
    return output_pos;
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
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer, sizeof(output_buffer));
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
        
        total_input += bytes_read;
        total_output += compressed_size;
    }
    
    file_info_t info;
    strncpy(info.filename, input_filename, MAX_FILENAME - 1);
    info.filename[MAX_FILENAME - 1] = '\0';
    info.original_size = total_input;
    info.compressed_size = total_output;
    
    fwrite(&info, sizeof(info), 1, output_file);
    
    fclose(input_file);
    fclose(output_file);
    
    printf("File processed: %s\n", input_filename);
    printf("Original size: %zu bytes\n", total_input);
    printf("Compressed size: %zu bytes\n", total_output);
    printf("Compression ratio: %.2f%%\n", total_input > 0 ? (double)total_output / total_input * 100 : 0.0);
    
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    if (!validate_filename(argv[1]) || !validate_filename(argv[2])) {
        printf("Error: Invalid filename\n");
        return 1;
    }
    
    if (strcmp(argv[1], argv[2]) == 0) {
        printf("Error: Input and output files must be different\n");
        return 1;
    }
    
    if (!process_file(argv[1], argv[2])) {
        printf("Error: Failed to process file\n");
        return 1;
    }
    
    return 0;
}