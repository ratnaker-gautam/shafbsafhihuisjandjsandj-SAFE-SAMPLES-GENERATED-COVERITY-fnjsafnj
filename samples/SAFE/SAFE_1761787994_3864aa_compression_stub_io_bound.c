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
            output[(*output_size)++] = value;
        }
        
        i += 2;
    }
    
    return 0;
}

int read_input(uint8_t* buffer, size_t* size) {
    if (buffer == NULL || size == NULL) {
        return -1;
    }
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    int c;
    
    while ((c = getchar()) != EOF && c != '\n' && total_read < MAX_INPUT_SIZE - 1) {
        buffer[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0) {
        printf("No input provided.\n");
        return -1;
    }
    
    *size = total_read;
    return 0;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    if (read_input(input_data, &input_size) != 0) {
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    Buffer compressed;
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = 0;
    
    if (decompress_rle(&compressed, decompressed, &decompressed_size) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed, decompressed_size);
    
    if (decompressed_size != input_size || memcmp(input_data, decompressed, input_size) != 0) {
        printf("Verification failed: decompressed data doesn't match original.\n");
        return 1;
    }
    
    printf("Verification successful. Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100.0);
    
    return 0;
}