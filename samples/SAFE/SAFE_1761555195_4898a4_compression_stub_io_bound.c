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
    if (ferror(file)) return -1;
    return 0;
}

int write_chunk(FILE *file, const Chunk *chunk) {
    if (file == NULL || chunk == NULL) return -1;
    if (chunk->size == 0) return 0;
    size_t written = fwrite(chunk->buffer, 1, chunk->size, file);
    if (written != chunk->size) return -1;
    return 0;
}

void compress_chunk(const Chunk *input, Chunk *output) {
    if (input == NULL || output == NULL) return;
    output->size = 0;
    if (input->size == 0) return;
    
    size_t in_pos = 0;
    size_t out_pos = 0;
    
    while (in_pos < input->size && out_pos < BUFFER_SIZE - 2) {
        uint8_t current = input->buffer[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               count < UCHAR_MAX && 
               input->buffer[in_pos + count] == current) {
            count++;
        }
        
        if (count > 2) {
            output->buffer[out_pos++] = 0xFF;
            output->buffer[out_pos++] = (uint8_t)count;
            output->buffer[out_pos++] = current;
            in_pos += count;
        } else {
            for (size_t i = 0; i < count && out_pos < BUFFER_SIZE; i++) {
                output->buffer[out_pos++] = current;
            }
            in_pos += count;
        }
    }
    output->size = out_pos;
}

int process_file(const char *input_filename, const char *output_filename) {
    if (input_filename == NULL || output_filename == NULL) return -1;
    if (strlen(input_filename) == 0 || strlen(output_filename) == 0) return -1;
    
    FILE *input = fopen(input_filename, "rb");
    if (input == NULL) return -1;
    
    FILE *output = fopen(output_filename, "wb");
    if (output == NULL) {
        fclose(input);
        return -1;
    }
    
    Chunk input_chunk, output_chunk;
    int result = 0;
    
    while (1) {
        if (read_chunk(input, &input_chunk) != 0) {
            result = -1;
            break;
        }
        
        if (input_chunk.size == 0) break;
        
        compress_chunk(&input_chunk, &output_chunk);
        
        if (write_chunk(output, &output_chunk) != 0) {
            result = -1;
            break;
        }
        
        if (input_chunk.size < BUFFER_SIZE) break;
    }
    
    fclose(input);
    fclose(output);
    return result;
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
    
    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "Input and output files must be different\n");
        return EXIT_FAILURE;
    }
    
    int result = process_file(argv[1], argv[2]);
    if (result != 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression completed successfully\n");
    return EXIT_SUCCESS;
}