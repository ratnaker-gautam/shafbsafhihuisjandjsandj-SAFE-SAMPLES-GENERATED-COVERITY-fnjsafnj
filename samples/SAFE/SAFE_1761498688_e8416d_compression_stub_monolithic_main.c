//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output->data[output->size++] = 0xFF;
            output->data[output->size++] = current;
            output->data[output->size++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && output->size < MAX_OUTPUT_SIZE; j++) {
                output->data[output->size++] = current;
            }
            i += count;
        }
    }
}

void decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count > 0) {
                size_t remaining = MAX_OUTPUT_SIZE - output->size;
                if (count > remaining) {
                    count = (uint8_t)remaining;
                }
                
                for (uint8_t j = 0; j < count; j++) {
                    output->data[output->size++] = value;
                }
            }
            
            i += 3;
        } else {
            output->data[output->size++] = input[i];
            i++;
        }
    }
}

int read_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    
    while (total_read < max_size - 1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        buffer[total_read++] = (uint8_t)c;
    }
    
    buffer[total_read] = '\0';
    return (int)total_read;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    int input_size = read_input(input, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("No input received or error reading input.\n");
        return 1;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);
    print_buffer_hex(input, (size_t)input_size);
    
    compress_rle(input, (size_t)input_size, &compressed);
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == (size_t)input_size && 
        memcmp(input, decompressed.data, decompressed.size) == 0) {
        printf("\nCompression/Decompression successful!\n");
        double ratio = (double)compressed.size / (double)input_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("\nCompression/Decompression failed!\n");
        return 1;
    }
    
    return 0;
}