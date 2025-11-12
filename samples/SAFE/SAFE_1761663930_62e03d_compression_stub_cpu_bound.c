//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_size + 3 > CHUNK_SIZE) break;
            output[output_size++] = 0xFF;
            output[output_size++] = (uint8_t)count;
            output[output_size++] = current;
        } else {
            if (output_size + count > CHUNK_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[output_size++] = current;
            }
        }
        
        i += count;
    }
    
    return output_size;
}

int process_compression(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        return 1;
    }
    
    uint8_t input_buffer[CHUNK_SIZE];
    uint8_t output_buffer[CHUNK_SIZE * 2];
    size_t bytes_read;
    
    while ((bytes_read = fread(input_buffer, 1, CHUNK_SIZE, input)) > 0) {
        size_t compressed_size = compress_chunk(input_buffer, bytes_read, output_buffer);
        
        if (compressed_size == 0) {
            return 1;
        }
        
        if (fwrite(&compressed_size, sizeof(size_t), 1, output) != 1) {
            return 1;
        }
        
        if (fwrite(output_buffer, 1, compressed_size, output) != compressed_size) {
            return 1;
        }
    }
    
    if (ferror(input)) {
        return 1;
    }
    
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    const char* input_filename = argv[1];
    const char* output_filename = argv[2];
    
    if (strlen(input_filename) == 0 || strlen(output_filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Cannot open input file\n");
        return 1;
    }
    
    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Cannot create output file\n");
        fclose(input_file);
        return 1;
    }
    
    if (fseek(input_file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Cannot determine file size\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    
    long file_size = ftell(input_file);
    if (file_size < 0 || file_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "File too large or invalid\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    
    if (fseek(input_file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Cannot reset file position\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    
    int result = process_compression(input_file, output_file);
    
    fclose(input_file);
    fclose(output_file);
    
    if (result != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compression completed successfully\n");
    return 0;
}