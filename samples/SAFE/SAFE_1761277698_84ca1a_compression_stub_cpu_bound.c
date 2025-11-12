//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

uint32_t simple_hash(const uint8_t* data, size_t len) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}

int compress_chunk(const Chunk* input, Chunk* output) {
    if (input == NULL || output == NULL || input->size > CHUNK_SIZE) {
        return -1;
    }
    
    if (input->size == 0) {
        output->size = 0;
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; ) {
        if (out_idx >= CHUNK_SIZE - 2) {
            return -1;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && 
               input->data[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && out_idx < CHUNK_SIZE; j++) {
                output->data[out_idx++] = current;
                i++;
            }
        }
    }
    
    output->size = out_idx;
    return 0;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL && size > 0) {
        return -1;
    }
    if (size > MAX_INPUT_SIZE) {
        return -1;
    }
    return 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t total_size = 0;
    
    printf("Enter data to compress (up to %d bytes):\n", MAX_INPUT_SIZE);
    
    int c;
    while (total_size < MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        if (c < 0 || c > 255) {
            fprintf(stderr, "Invalid input byte\n");
            return 1;
        }
        input_buffer[total_size++] = (uint8_t)c;
    }
    
    if (total_size == 0) {
        fprintf(stderr, "No input data\n");
        return 1;
    }
    
    if (validate_input(input_buffer, total_size) != 0) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    uint8_t output_buffer[MAX_INPUT_SIZE * 2];
    size_t output_size = 0;
    size_t processed = 0;
    
    while (processed < total_size) {
        Chunk input_chunk;
        Chunk output_chunk;
        
        size_t chunk_size = total_size - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        memcpy(input_chunk.data, input_buffer + processed, chunk_size);
        input_chunk.size = chunk_size;
        
        if (compress_chunk(&input_chunk, &output_chunk) != 0) {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
        
        if (output_size + output_chunk.size > sizeof(output_buffer)) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        memcpy(output_buffer + output_size, output_chunk.data, output_chunk.size);
        output_size += output_chunk.size;
        processed += chunk_size;
    }
    
    uint32_t input_hash = simple_hash(input_buffer, total_size);
    uint32_t output_hash = simple_hash(output_buffer, output_size);
    
    printf("Original size: %zu bytes\n", total_size);
    printf("Compressed size: %zu bytes\n", output_size);
    printf("Compression ratio: %.2f%%\n", 
           (double)output_size / total_size * 100.0);
    printf("Input checksum: %u\n", input_hash);
    printf("Output checksum: %u\n", output_hash);
    
    printf("Compressed data (first 100 bytes): ");
    for (size_t i = 0; i < output_size && i < 100; i++) {
        printf("%02X ", output_buffer[i]);
    }
    printf("\n");
    
    return 0;
}