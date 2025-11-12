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

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (i + 1 >= input->size) {
            return -1;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size + j] = value;
        }
        
        *output_size += count;
        i += 2;
    }
    
    return 0;
}

int read_input(uint8_t* buffer, size_t* size) {
    if (buffer == NULL || size == NULL) {
        return -1;
    }
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t pos = 0;
    int c;
    
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE - 1) {
        if (isprint(c) || c == ' ' || c == '\t') {
            buffer[pos++] = (uint8_t)c;
        }
    }
    
    buffer[pos] = '\0';
    *size = pos;
    
    return 0;
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
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    if (read_input(input, &input_size) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original text: %s\n", input);
    printf("Original size: %zu bytes\n", input_size);
    
    if (compress_rle(input, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    if (decompress_rle(&compressed, decompressed, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    decompressed[decompressed_size] = '\0';
    printf("Decompressed text: %s\n", decompressed);
    printf("Decompressed size: %zu bytes\n", decompressed_size);
    
    if (input_size != decompressed_size || memcmp(input, decompressed, input_size) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / (double)input_size) * 100.0);
    
    return 0;
}