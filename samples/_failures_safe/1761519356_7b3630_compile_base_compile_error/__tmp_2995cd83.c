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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (output_pos + 3 > input_size * 2) {
                break;
            }
            output[output_pos++] = 0xFF;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && output_pos < input_size * 2; j++) {
                output[output_pos++] = input[i + j];
            }
            i += count;
        }
    }
    
    return output_pos;
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
        size_t compressed_size = simple_compress(input_buffer, bytes_read, output_buffer);
        
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
    
    file_info_t info;
    strncpy(info.filename, input_filename, MAX_FILENAME - 1);
    info.filename[MAX_FILENAME - 1] = '\0';
    info.original_size = total_input;
    info.compressed_size = total_output;
    
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
        return 1;
    }
    
    size_t len = strlen(input_file);
    if (len > 0 && input_file[len - 1] == '\n') {
        input_file[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (fgets(output_file, MAX_FILENAME, stdin) == NULL) {
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
    
    if (process_file(input_file, output_file)) {
        printf("Compression completed successfully\n");
        
        FILE* result_file = fopen(output_file, "rb");
        if (result_file != NULL) {
            if (fseek(result_file, -((long)sizeof(file_info_t)), SEEK_END) == 0) {
                file_info_t info;
                if (fread(&info, sizeof(file_info_t), 1, result_file) ==