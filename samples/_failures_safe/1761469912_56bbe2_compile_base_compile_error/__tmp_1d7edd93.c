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
        uint8_t current = input[i];
        size_t count = 1;

        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }

        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 > *output_size) return -1;
            output[out_idx++] = 0xFF;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (out_idx >= *output_size) return -1;
                output[out_idx++] = current;
            }
        }

        i += count;
    }

    *output_size = out_idx;
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
    
    if (strlen(input_filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
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
    
    if (strlen(output_filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
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
    if (file_size < 0 || file_size > SIZE_MAX) {
        fprintf(stderr, "Invalid file size\n");
        fclose(input_file);
        return 1;
    }
    
    rewind(input_file);
    
    size_t data_size = (size_t)file_size;
    uint8_t* input_data = malloc(data_size);
    if (input_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(input_file);
        return 1;
    }
    
    size_t bytes_read = fread(input_data, 1, data_size, input_file);
    fclose(input_file);
    
    if (bytes_read != data_size) {
        fprintf(stderr, "Error reading file\n");
        free(input_data);
        return 1;
    }
    
    size_t max_output_size = data_size * 2;
    uint8_t* output_data = malloc(max_output_size);
    if (output_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(input_data);
        return 1;
    }
    
    size_t compressed_size = max_output_size;
    if (compress_data(input_data, data_size, output_data, &compressed_size) != 0) {
        fprintf(stderr, "Compression failed\n");
        free(input_data);
        free(output_data);
        return 1;
    }
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Cannot create output file\n");
        free(input_data);
        free(output_data);
        return 1;
    }
    
    FileHeader header;
    strncpy(header.filename, input_filename, sizeof(header.filename) - 1);
    header.filename[sizeof(header.filename) - 1] = '\0';
    header.original_size = data_size;
    header.compressed_size = compressed_size;
    
    if (fwrite(&header, sizeof(header), 1, output_file) != 1) {
        fprintf(stderr, "Error writing header\n");
        fclose(output_file);
        free(input_data);
        free(output_data);
        return 1;
    }
    
    if (fwrite(output_data, 1, compressed_size, output_file) != compressed_size) {
        fprintf(stderr,