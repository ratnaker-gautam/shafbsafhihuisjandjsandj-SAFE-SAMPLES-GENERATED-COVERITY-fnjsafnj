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
    if (written != chunk->size || ferror(file)) return -1;
    return 0;
}

int compress_chunk(const Chunk *input, Chunk *output) {
    if (input == NULL || output == NULL) return -1;
    if (input->size > BUFFER_SIZE) return -1;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size && out_idx < BUFFER_SIZE - 2; ) {
        uint8_t current = input->buffer[i];
        size_t count = 1;
        
        while (i + count < input->size && count < UINT8_MAX && 
               input->buffer[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (out_idx + 2 >= BUFFER_SIZE) return -1;
            output->buffer[out_idx++] = 0xFF;
            output->buffer[out_idx++] = (uint8_t)count;
            output->buffer[out_idx++] = current;
            i += count;
        } else {
            output->buffer[out_idx++] = current;
            i++;
        }
    }
    output->size = out_idx;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
        return 1;
    }
    
    if (strlen(argv[1]) >= MAX_FILENAME || strlen(argv[2]) >= MAX_FILENAME) {
        fprintf(stderr, "Filename too long\n");
        return 1;
    }
    
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL) {
        fprintf(stderr, "Cannot open input file\n");
        return 1;
    }
    
    FILE *output = fopen(argv[2], "wb");
    if (output == NULL) {
        fprintf(stderr, "Cannot create output file\n");
        fclose(input);
        return 1;
    }
    
    uint8_t header[4] = {'C', 'M', 'P', 0x01};
    if (fwrite(header, 1, 4, output) != 4) {
        fprintf(stderr, "Failed to write header\n");
        fclose(input);
        fclose(output);
        return 1;
    }
    
    Chunk input_chunk, output_chunk;
    int result = 0;
    
    while (1) {
        if (read_chunk(input, &input_chunk) != 0) {
            fprintf(stderr, "Error reading input\n");
            result = 1;
            break;
        }
        
        if (input_chunk.size == 0) break;
        
        if (compress_chunk(&input_chunk, &output_chunk) != 0) {
            fprintf(stderr, "Compression error\n");
            result = 1;
            break;
        }
        
        if (write_chunk(output, &output_chunk) != 0) {
            fprintf(stderr, "Error writing output\n");
            result = 1;
            break;
        }
        
        if (input_chunk.size < BUFFER_SIZE) break;
    }
    
    fclose(input);
    fclose(output);
    return result;
}