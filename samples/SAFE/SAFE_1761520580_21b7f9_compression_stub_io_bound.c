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
    if (chunk->size > 0) {
        size_t written = fwrite(chunk->buffer, 1, chunk->size, file);
        if (written != chunk->size || ferror(file)) return -1;
    }
    return 0;
}

int compress_chunk(const Chunk *input, Chunk *output) {
    if (input == NULL || output == NULL) return -1;
    if (input->size > BUFFER_SIZE) return -1;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size && out_pos < BUFFER_SIZE - 2) {
        uint8_t current = input->buffer[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               count < 255 && 
               input->buffer[in_pos + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_pos + 2 >= BUFFER_SIZE) break;
            output->buffer[out_pos++] = 0xFF;
            output->buffer[out_pos++] = current;
            output->buffer[out_pos++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count && out_pos < BUFFER_SIZE; i++) {
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
    
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL) {
        fprintf(stderr, "Cannot open input file\n");
        return EXIT_FAILURE;
    }
    
    FILE *output = fopen(argv[2], "wb");
    if (output == NULL) {
        fprintf(stderr, "Cannot create output file\n");
        fclose(input);
        return EXIT_FAILURE;
    }
    
    Chunk input_chunk, compressed_chunk;
    int result = 0;
    
    while (!feof(input) && result == 0) {
        if (read_chunk(input, &input_chunk) != 0) {
            result = -1;
            break;
        }
        
        if (input_chunk.size == 0) break;
        
        if (compress_chunk(&input_chunk, &compressed_chunk) != 0) {
            result = -1;
            break;
        }
        
        if (write_chunk(output, &compressed_chunk) != 0) {
            result = -1;
            break;
        }
    }
    
    if (fflush(output) != 0) result = -1;
    
    fclose(input);
    fclose(output);
    
    if (result != 0) {
        fprintf(stderr, "Compression failed\n");
        remove(argv[2]);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}