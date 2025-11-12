//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_index + 3 >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_index++] = 0xFF;
            output[output_index++] = current;
            output[output_index++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (output_index >= MAX_OUTPUT_SIZE) {
                    return 0;
                }
                output[output_index++] = current;
            }
        }
        
        i += count;
    }
    
    return output_index;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output_index + count > MAX_INPUT_SIZE) {
                return 0;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_index++] = value;
            }
            
            i += 3;
        } else {
            if (output_index >= MAX_INPUT_SIZE) {
                return 0;
            }
            output[output_index++] = input[i];
            i++;
        }
    }
    
    return output_index;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    if (buf1 == NULL || buf2 == NULL) {
        return 0;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    size_t bytes_read = 0;
    
    while ((c = getchar()) != EOF && bytes_read < MAX_INPUT_SIZE) {
        input_buffer.data[bytes_read++] = (uint8_t)c;
    }
    
    if (bytes_read == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    input_buffer.size = bytes_read;
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        printf("Invalid input data.\n");
        return 1;
    }
    
    compressed_buffer.size = simple_compress(input_buffer.data, input_buffer.size, compressed_buffer.data);
    
    if (compressed_buffer.size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buffer.size);
    printf("Compressed size: %zu bytes\n", compressed_buffer.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buffer.size / input_buffer.size) * 100);
    
    decompressed_buffer.size = simple_decompress(compressed_buffer.data, compressed_buffer.size, decompressed_buffer.data);
    
    if (decompressed_buffer.size == 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (decompressed_buffer.size != input_buffer.size) {
        printf("Decompressed size mismatch.\n");
        return 1;
    }
    
    if (!compare_buffers(input_buffer.data, decompressed_buffer.data, input_buffer.size)) {
        printf("Decompression verification failed.\n");
        return 1;
    }
    
    printf("Compression and decompression successful.\n");
    printf("Data integrity verified.\n");
    
    return 0;
}