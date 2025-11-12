//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_idx + 2 > CHUNK_SIZE) break;
            output[output_idx++] = 0xFF;
            output[output_idx++] = (uint8_t)count;
            output[output_idx++] = current;
        } else {
            if (output_idx + count > CHUNK_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

int process_compression(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    
    uint8_t input_buffer[CHUNK_SIZE];
    uint8_t output_buffer[CHUNK_SIZE];
    size_t total_read = 0;
    
    while (!feof(input)) {
        if (total_read >= MAX_INPUT_SIZE) {
            return 0;
        }
        
        size_t bytes_read = fread(input_buffer, 1, CHUNK_SIZE, input);
        if (ferror(input)) {
            return 0;
        }
        
        if (bytes_read == 0) {
            break;
        }
        
        total_read += bytes_read;
        
        size_t compressed_size = compress_chunk(input_buffer, bytes_read, output_buffer);
        
        uint32_t chunk_header = (uint32_t)compressed_size;
        if (fwrite(&chunk_header, sizeof(uint32_t), 1, output) != 1) {
            return 0;
        }
        
        if (compressed_size > 0) {
            if (fwrite(output_buffer, 1, compressed_size, output) != compressed_size) {
                return 0;
            }
        }
    }
    
    uint32_t end_marker = 0xFFFFFFFF;
    if (fwrite(&end_marker, sizeof(uint32_t), 1, output) != 1) {
        return 0;
    }
    
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    FILE* input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Cannot open input file\n");
        return 1;
    }
    
    FILE* output_file = fopen(argv[2], "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Error: Cannot create output file\n");
        fclose(input_file);
        return 1;
    }
    
    int result = process_compression(input_file, output_file);
    
    fclose(input_file);
    fclose(output_file);
    
    if (!result) {
        fprintf(stderr, "Error: Compression failed\n");
        remove(argv[2]);
        return 1;
    }
    
    return 0;
}