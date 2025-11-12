//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size % 2 != 0) {
        return -1;
    }
    
    *output_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size] = value;
            (*output_size)++;
        }
    }
    
    return 0;
}

int read_input(uint8_t* buffer, size_t* size) {
    if (buffer == NULL || size == NULL) {
        return -1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    size_t pos = 0;
    int c;
    
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE) {
        buffer[pos++] = (uint8_t)c;
    }
    
    if (pos == 0) {
        printf("No input provided.\n");
        return -1;
    }
    
    *size = pos;
    return 0;
}

void print_buffer(const uint8_t* buffer, size_t size) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (isprint(buffer[i])) {
            printf("%c", buffer[i]);
        } else {
            printf("\\x%02x", buffer[i]);
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    if (read_input(input, &input_size) != 0) {
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    print_buffer(input, input_size);
    
    if (compress_rle(input, input_size, &compressed) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
    
    if (decompress_rle(&compressed, decompressed, &decompressed_size) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    print_buffer(decompressed, decompressed_size);
    
    if (input_size != decompressed_size || memcmp(input, decompressed, input_size) != 0) {
        printf("Verification failed: original and decompressed data don't match.\n");
        return 1;
    }
    
    printf("Verification successful. Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / (double)input_size) * 100.0);
    
    return 0;
}