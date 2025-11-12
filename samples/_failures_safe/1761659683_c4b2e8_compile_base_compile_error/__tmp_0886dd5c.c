//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define CHUNK_SIZE 16
#define COMPRESSION_THRESHOLD 4
#define RUN_LENGTH_MASK 0x7F
#define MAX_RUN_LENGTH 127

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < chunk_size) {
        uint8_t current = chunk[i];
        size_t run_length = 1;
        
        while (i + run_length < chunk_size && 
               chunk[i + run_length] == current && 
               run_length <= MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (output_idx + 2 > MAX_OUTPUT_SIZE) break;
            output[output_idx++] = (uint8_t)(run_length | 0x80);
            output[output_idx++] = current;
            i += run_length;
        } else {
            size_t literal_count = 0;
            while (i + literal_count < chunk_size && 
                   literal_count < run_length && 
                   literal_count <= MAX_RUN_LENGTH) {
                if (i + literal_count + 1 < chunk_size && 
                    chunk[i + literal_count] == chunk[i + literal_count + 1] &&
                    literal_count + 2 >= COMPRESSION_THRESHOLD) {
                    break;
                }
                literal_count++;
            }
            
            if (output_idx + literal_count + 1 > MAX_OUTPUT_SIZE) break;
            output[output_idx++] = (uint8_t)literal_count;
            for (size_t j = 0; j < literal_count; j++) {
                output[output_idx++] = chunk[i + j];
            }
            i += literal_count;
        }
    }
    
    return output_idx;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < chunk_size) {
        if (output_idx >= MAX_OUTPUT_SIZE) break;
        
        uint8_t control = chunk[i++];
        
        if (control & 0x80) {
            size_t run_length = control & RUN_LENGTH_MASK;
            if (i >= chunk_size) break;
            uint8_t value = chunk[i++];
            
            if (output_idx + run_length > MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < run_length; j++) {
                output[output_idx++] = value;
            }
        } else {
            size_t literal_count = control;
            
            if (i + literal_count > chunk_size) break;
            if (output_idx + literal_count > MAX_OUTPUT_SIZE) break;
            
            for (size_t j = 0; j < literal_count; j++) {
                output[output_idx++] = chunk[i++];
            }
        }
    }
    
    return output_idx;
}

static void process_compression(const uint8_t* input, size_t input_size, Buffer* compressed) {
    if (!validate_input(input, input_size)) return;
    
    compressed->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size && compressed->size < MAX_OUTPUT_SIZE) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        
        size_t compressed_size = compress_chunk(input + total_processed, chunk_size, 
                                              compressed->data + compressed->size);
        
        if (compressed_size == 0) break;
        
        compressed->size += compressed_size;
        total_processed += chunk_size;
    }
}

static void process_decompression(const uint8_t* input, size_t input_size, Buffer* decompressed) {
    if (!validate_input(input, input_size)) return;
    
    decompressed->size = decompress_chunk(input, input_size, decompressed->data);
}

static void print_buffer_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t test_input[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    for (size_t i = 0; i < sizeof(test_input); i++) {
        test_input[i] = (uint