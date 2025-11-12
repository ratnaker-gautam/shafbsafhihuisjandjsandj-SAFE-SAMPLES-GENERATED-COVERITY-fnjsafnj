//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 4096
#define MAX_FILENAME 256

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    size_t size;
} Chunk;

int read_chunk(FILE *file, Chunk *chunk) {
    if (file == NULL || chunk == NULL) return -1;
    
    chunk->size = fread(chunk->buffer, 1, BUFFER_SIZE, file);
    if (chunk->size == 0 && ferror(file)) return -1;
    
    return 0;
}

int write_chunk(FILE *file, Chunk *chunk) {
    if (file == NULL || chunk == NULL) return -1;
    
    size_t written = fwrite(chunk->buffer, 1, chunk->size, file);
    if (written != chunk->size) return -1;
    
    return 0;
}

int compress_chunk(Chunk *input, Chunk *output) {
    if (input == NULL || output == NULL) return -1;
    if (input->size > BUFFER_SIZE) return -1;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        if (out_pos >= BUFFER_SIZE - 2) return -1;
        
        uint8_t current = input->buffer[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               count < UCHAR_MAX && 
               input->buffer[in_pos + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output->buffer[out_pos++] = 0xFF;
            output->buffer[out_pos++] = current;
            output->buffer[out_pos++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count; i++) {
                output->buffer[out_pos++] = current;
            }
        }
        
        in_pos += count;
    }
    
    output->size = out_pos;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (strlen(argv[1]) >= MAX_FILENAME || strlen(argv[2]) >= MAX_FILENAME) {
        fprintf(stderr, "Filename too long\n");
        return EXIT_FAILURE;
    }
    
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Cannot open input file\n");
        return EXIT_FAILURE;
    }
    
    FILE *output_file = fopen(argv[2], "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Cannot open output file\n");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    
    Chunk input_chunk, output_chunk;
    int result = EXIT_SUCCESS;
    
    while (1) {
        if (read_chunk(input_file, &input_chunk) != 0) {
            if (ferror(input_file)) {
                fprintf(stderr, "Error reading input file\n");
                result = EXIT_FAILURE;
            }
            break;
        }
        
        if (input_chunk.size == 0) break;
        
        if (compress_chunk(&input_chunk, &output_chunk) != 0) {
            fprintf(stderr, "Compression error\n");
            result = EXIT_FAILURE;
            break;
        }
        
        if (write_chunk(output_file, &output_chunk) != 0) {
            fprintf(stderr, "Error writing output file\n");
            result = EXIT_FAILURE;
            break;
        }
    }
    
    if (fclose(input_file) != 0) {
        fprintf(stderr, "Error closing input file\n");
        result = EXIT_FAILURE;
    }
    
    if (fclose(output_file) != 0) {
        fprintf(stderr, "Error closing output file\n");
        result = EXIT_FAILURE;
    }
    
    return result;
}