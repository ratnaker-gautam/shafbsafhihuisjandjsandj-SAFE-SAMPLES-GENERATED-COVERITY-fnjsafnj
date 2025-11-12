//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }
    }
    
    return 0;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    printf("RLE Compression/Decompression Tool\n");
    printf("Enter 'c' to compress or 'd' to decompress: ");
    
    char mode;
    if (scanf(" %c", &mode) != 1) {
        printf("Error reading mode\n");
        return 1;
    }
    
    if (mode != 'c' && mode != 'd') {
        printf("Invalid mode. Use 'c' for compress or 'd' for decompress.\n");
        return 1;
    }
    
    printf("Enter data as hex bytes (e.g., 41 42 43): ");
    
    Buffer input_buffer = {0};
    Buffer output_buffer = {0};
    
    char hex[3] = {0};
    int byte;
    
    while (scanf("%2s", hex) == 1) {
        if (input_buffer.size >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return 1;
        }
        
        if (sscanf(hex, "%02x", &byte) != 1) {
            printf("Invalid hex byte: %s\n", hex);
            return 1;
        }
        
        input_buffer.data[input_buffer.size++] = (uint8_t)byte;
        
        int next_char = getchar();
        if (next_char == '\n' || next_char == EOF) {
            break;
        }
    }
    
    if (input_buffer.size == 0) {
        printf("No input data provided\n");
        return 1;
    }
    
    int result;
    if (mode == 'c') {
        result = compress_rle(input_buffer.data, input_buffer.size, &output_buffer);
        if (result == 0) {
            printf("Compressed data (%zu bytes):\n", output_buffer.size);
            print_hex(output_buffer.data, output_buffer.size);
            printf("Compression ratio: %.2f%%\n", 
                   (1.0 - (double)output_buffer.size / input_buffer.size) * 100);
        } else {
            printf("Compression failed\n");
            return 1;
        }
    } else {
        result = decompress_rle(input_buffer.data, input_buffer.size, &output_buffer);
        if (result == 0) {
            printf("Decompressed data (%zu bytes):\n", output_buffer.size);
            print_hex(output_buffer.data, output_buffer.size);
        } else {
            printf("Decompression failed\n");
            return 1;
        }
    }
    
    return 0;
}