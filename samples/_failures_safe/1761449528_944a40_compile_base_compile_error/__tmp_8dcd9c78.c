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

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return -1;
    if (input_size == 0) return -1;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < *output_size - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            output[out_idx++] = current;
            in_idx++;
        }
    }
    
    if (in_idx < input_size) return -1;
    
    *output_size = out_idx;
    return 0;
}

int validate_filename(const char* filename) {
    if (filename == NULL) return -1;
    size_t len = strlen(filename);
    if (len == 0 || len >= MAX_FILENAME) return -1;
    
    for (size_t i = 0; i < len; i++) {
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':' || 
            filename[i] == '*' || filename[i] == '?' || filename[i] == '"' || 
            filename[i] == '<' || filename[i] == '>' || filename[i] == '|') {
            return -1;
        }
    }
    return 0;
}

int main(void) {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (fgets(input_filename, sizeof(input_filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    if (validate_filename(input_filename) != 0) {
        fprintf(stderr, "Invalid input filename\n");
        return 1;
    }
    
    printf("Enter output filename: ");
    if (fgets(output_filename, sizeof(output_filename), stdin) == NULL) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (validate_filename(output_filename) != 0) {
        fprintf(stderr, "Invalid output filename\n");
        return 1;
    }
    
    if (strcmp(input_filename, output_filename) == 0) {
        fprintf(stderr, "Input and output filenames cannot be the same\n");
        return 1;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Cannot open input file\n");
        return 1;
    }
    
    if (fseek(input_file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error seeking input file\n");
        fclose(input_file);
        return 1;
    }
    
    long file_size = ftell(input_file);
    if (fseek(input_file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking input file\n");
        fclose(input_file);
        return 1;
    }
    
    if (file_size <= 0 || file_size > INT_MAX) {
        fprintf(stderr, "Invalid file size\n");
        fclose(input_file);
        return 1;
    }
    
    uint8_t* input_data = malloc(file_size);
    if (input_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(input_file);
        return 1;
    }
    
    size_t bytes_read = fread(input_data, 1, file_size, input_file);
    fclose(input_file);
    
    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Error reading input file\n");
        free(input_data);
        return 1;
    }
    
    size_t max_output_size = file_size * 2;
    if (max_output_size < file_size) {
        fprintf(stderr, "File too large\n");
        free(input_data);
        return 1;
    }
    
    uint8_t* output_data = malloc(max_output_size);
    if (output_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(input_data);
        return 1;
    }
    
    size_t compressed_size = max_output_size;
    if (compress_data(input_data, file_size, output_data, &compressed_size