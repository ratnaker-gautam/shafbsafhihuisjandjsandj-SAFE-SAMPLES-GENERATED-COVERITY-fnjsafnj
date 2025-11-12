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
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
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
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }
        
        i += 2;
    }
    
    return 0;
}

int read_input(Buffer* buffer) {
    if (buffer == NULL) {
        return -1;
    }
    
    buffer->size = 0;
    int c;
    
    while ((c = getchar()) != EOF && buffer->size < MAX_INPUT_SIZE) {
        buffer->data[buffer->size++] = (uint8_t)c;
    }
    
    if (buffer->size == 0) {
        return -1;
    }
    
    return 0;
}

void write_output(const Buffer* buffer) {
    if (buffer == NULL || buffer->size == 0) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        putchar(buffer->data[i]);
    }
}

int main(void) {
    Buffer input_data;
    Buffer compressed;
    Buffer decompressed;
    
    printf("Enter text to compress (max %d bytes):\n", MAX_INPUT_SIZE);
    
    if (read_input(&input_data) != 0) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    if (compress_rle(input_data.data, input_data.size, &compressed) != 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    write_output(&compressed);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Error: Decompression failed\n");
        return 1;
    }
    
    printf("\n\nDecompressed data (%zu bytes):\n", decompressed.size);
    write_output(&decompressed);
    
    if (input_data.size != decompressed.size || 
        memcmp(input_data.data, decompressed.data, input_data.size) != 0) {
        fprintf(stderr, "Error: Data mismatch after compression/decompression\n");
        return 1;
    }
    
    printf("\n\nCompression successful. Original: %zu bytes, Compressed: %zu bytes\n", 
           input_data.size, compressed.size);
    
    return 0;
}