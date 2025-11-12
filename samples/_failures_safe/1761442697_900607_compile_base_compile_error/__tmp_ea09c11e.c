//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define CHUNK_SIZE 16
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR 0xFF

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_len, uint8_t* output) {
    if (chunk_len == 0) return 0;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < chunk_len) {
        uint8_t current = chunk[i];
        size_t count = 1;
        
        while (i + count < chunk_len && chunk[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (out_pos + 3 > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = ESCAPE_CHAR;
            output[out_pos++] = (uint8_t)count;
            output[out_pos++] = current;
        } else {
            if (out_pos + count > MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[out_pos++] = current;
            }
        }
        
        i += count;
    }
    
    return out_pos;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_len, uint8_t* output) {
    if (chunk_len == 0) return 0;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < chunk_len) {
        if (chunk[i] == ESCAPE_CHAR && i + 2 < chunk_len) {
            uint8_t count = chunk[i + 1];
            uint8_t value = chunk[i + 2];
            
            if (out_pos + count > MAX_OUTPUT_SIZE) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_pos++] = value;
            }
            
            i += 3;
        } else {
            if (out_pos + 1 > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = chunk[i++];
        }
    }
    
    return out_pos;
}

static int compress_data(const uint8_t* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len)) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_len) {
        size_t remaining = input_len - total_processed;
        size_t chunk_len = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        
        uint8_t compressed_chunk[MAX_OUTPUT_SIZE];
        size_t compressed_size = compress_chunk(input + total_processed, chunk_len, compressed_chunk);
        
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        
        memcpy(output->data + output->size, compressed_chunk, compressed_size);
        output->size += compressed_size;
        total_processed += chunk_len;
    }
    
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len)) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_len) {
        size_t remaining = input_len - total_processed;
        size_t chunk_len = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        
        uint8_t decompressed_chunk[MAX_OUTPUT_SIZE];
        size_t decompressed_size = decompress_chunk(input + total_processed, chunk_len, decompressed_chunk);
        
        if (output->size + decompressed_size > MAX_OUTPUT_SIZE) return 0;
        
        memcpy(output->data + output->size, decompressed_chunk, decompressed_size);
        output->size += decompressed_size;
        total_processed += chunk_len;
    }
    
    return 1;
}

static void print_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < len) printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t test_input[] = "AAAABBBBCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPP";
    size_t input_len = sizeof(test_input) - 1;
    
    printf("Original data (%zu bytes):\n", input_len);
    print_hex(test_input, input_len);
    
    Buffer compressed;
    if (!compress