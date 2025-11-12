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
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output_pos >= output_size - 2) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output[output_pos++] = 0xFF;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[output_pos++] = input[i++];
            }
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
    
    file_info_t info;
    strncpy(info.filename, input_filename, MAX_FILENAME - 1);
    info.filename[MAX_FILENAME - 1] = '\0';
    info.original_size = 0;
    info.compressed_size = 0;
    
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file);
        if (bytes_read == 0) {
            break;
        }
        
        info.original_size += bytes_read;
        
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
        
        info.compressed_size += compressed_size;
    }
    
    fwrite(&info, sizeof(info), 1, output_file);
    
    fclose(input_file);
    fclose(output_file);
    
    return 1;
}

int main(void) {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_filename, sizeof(input_filename), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_filename, sizeof(output_filename), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        printf("Error: Invalid filename\n");
        return EXIT_FAILURE;
    }
    
    if (strcmp(input_filename, output_filename) == 0) {
        printf("Error: Input and output filenames cannot be the same\n");
        return EXIT_FAILURE;
    }
    
    printf("Processing file...\n");
    
    if (process_file(input_filename, output_filename)) {
        printf("Compression completed successfully\n");
        return EXIT_SUCCESS;
    } else {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
}