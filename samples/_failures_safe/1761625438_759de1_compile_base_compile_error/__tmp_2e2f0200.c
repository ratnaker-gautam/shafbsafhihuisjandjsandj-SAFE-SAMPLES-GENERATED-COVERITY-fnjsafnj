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
    if (input_size == 0 || *output_size < input_size) return -1;

    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        if (out_idx >= *output_size - 2) return -1;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    *output_size = out_idx;
    return 0;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return -1;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        if (i + 1 >= input_size) return -1;
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > *output_size) return -1;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    *output_size = out_idx;
    return 0;
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    int mode;
    
    printf("Enter input filename: ");
    if (fgets(input_filename, sizeof(input_filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
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
    
    printf("Select mode (1=compress, 2=decompress): ");
    if (scanf("%d", &mode) != 1) {
        fprintf(stderr, "Error reading mode\n");
        return 1;
    }
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (mode != 1 && mode != 2) {
        fprintf(stderr, "Invalid mode selected\n");
        return 1;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Cannot open input file\n");
        return 1;
    }
    
    if (fseek(input_file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error seeking file\n");
        fclose(input_file);
        return 1;
    }
    
    long file_size = ftell(input_file);
    if (fseek(input_file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking file\n");
        fclose(input_file);
        return 1;
    }
    
    if (file_size <= 0 || file_size > INT_MAX) {
        fprintf(stderr, "Invalid file size\n");
        fclose(input_file);
        return 1;
    }
    
    size_t input_size = (size_t)file_size;
    uint8_t* input_data = malloc(input_size);
    if (input_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(input_file);
        return 1;
    }
    
    if (fread(input_data, 1, input_size, input_file) != input_size) {
        fprintf(stderr, "Error reading input file\n");
        free(input_data);
        fclose(input_file);
        return 1;
    }
    fclose(input_file);
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Cannot create output file\n");
        free(input_data);
        return 1;
    }
    
    int result = 0;
    
    if (mode == 1) {
        size_t max_output_size = input_size * 2;
        if (max_output_size < input_size) {
            fprintf(stderr, "File too large for compression\n");
            free(input_data);
            fclose(output_file);
            return 1;
        }
        
        uint8_t* compressed_data = malloc(max_output_size);