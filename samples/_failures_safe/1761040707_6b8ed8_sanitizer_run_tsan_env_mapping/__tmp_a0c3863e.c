//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MARKER 0xFF
#define MAX_RUN_LENGTH 255
#define MIN_RUN_LENGTH 3

typedef struct {
    uint8_t data[MAX_INPUT_SIZE * 2];
    size_t size;
} Buffer;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > sizeof(output->data)) return 0;
        
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && 
               run_length < MAX_RUN_LENGTH && 
               input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= MIN_RUN_LENGTH) {
            output->data[output->size++] = COMPRESSED_MARKER;
            output->data[output->size++] = (uint8_t)run_length;
            output->data[output->size++] = current;
            i += run_length;
        } else {
            output->data[output->size++] = current;
            i++;
        }
    }
    
    return output->size;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size >= sizeof(output->data)) return 0;
        
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_size) return 0;
            
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (run_length < MIN_RUN_LENGTH) return 0;
            if (output->size + run_length > sizeof(output->data)) return 0;
            
            for (uint8_t j = 0; j < run_length; j++) {
                output->data[output->size++] = value;
            }
            
            i += 3;
        } else {
            output->data[output->size++] = input[i];
            i++;
        }
    }
    
    return output->size;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x43, 0x44, 0x45, 0x45, 0x45, 0x45, 0x45};
    size_t test_size = sizeof(test_data);
    
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Original data (%zu bytes):\n", test_size);
    print_buffer_hex((Buffer*)&test_data);
    
    size_t compressed_size = compress_rle(test_data, test_size, &compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(&compressed);
    
    size_t decompressed_size = decompress_rle(compressed.data, compressed_size, &decompressed);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    print_buffer_hex(&decompressed);
    
    if (decompressed_size != test_size) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(test_data, decompressed.data, test_size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / test_size) * 100.0);
    
    return 0;
}