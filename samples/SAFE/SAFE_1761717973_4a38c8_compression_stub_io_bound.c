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

int decompress_rle(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < output_size) {
        if (in_idx + 1 >= input->size) {
            return -1;
        }
        
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > output_size) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return 0;
}

int read_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return -1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    
    while ((c = getchar()) != EOF && c != '\n' && total_read < max_size - 1) {
        buffer[total_read++] = (uint8_t)c;
    }
    
    buffer[total_read] = '\0';
    return (int)total_read;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        printf("(empty)\n");
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
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    int input_size = read_input(input, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("Error: No input data received\n");
        return 1;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);
    print_buffer_hex(input, (size_t)input_size);
    
    if (compress_rle(input, (size_t)input_size, &compressed) != 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_size * 100.0f);
    
    if (decompress_rle(&compressed, decompressed, MAX_INPUT_SIZE) != 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%d bytes):\n", input_size);
    print_buffer_hex(decompressed, (size_t)input_size);
    
    if (memcmp(input, decompressed, (size_t)input_size) == 0) {
        printf("\nVerification: SUCCESS - Original and decompressed data match\n");
    } else {
        printf("\nVerification: FAILED - Data mismatch detected\n");
        return 1;
    }
    
    return 0;
}