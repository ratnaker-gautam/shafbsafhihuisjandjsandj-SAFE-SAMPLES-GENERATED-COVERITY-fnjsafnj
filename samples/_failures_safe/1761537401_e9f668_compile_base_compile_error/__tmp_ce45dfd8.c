//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size * 2) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_idx + 2 > output_size) return 0;
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 1 >= input_size) return 0;
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
        
        i += 2;
    }
    
    return output_idx;
}

int read_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char temp[MAX_INPUT_SIZE * 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) return 0;
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_SIZE) return 0;
    
    memcpy(buffer->data, temp, len);
    buffer->size = len;
    
    return 1;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed_data = {0};
    Buffer decompressed_data = {0};
    
    if (!read_input(&input_data)) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (!validate_input(input_data.data, input_data.size)) {
        printf("Invalid input\n");
        return 1;
    }
    
    compressed_data.size = simple_compress(input_data.data, input_data.size, 
                                         compressed_data.data, sizeof(compressed_data.data));
    
    if (compressed_data.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_data.size);
    printf("Compressed size: %zu bytes\n", compressed_data.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_data.size / input_data.size) * 100.0);
    
    printf("Compressed data (hex):\n");
    print_buffer_hex(compressed_data.data, compressed_data.size);
    
    decompressed_data.size = simple_decompress(compressed_data.data, compressed_data.size,
                                             decompressed_data.data, sizeof(decompressed_data.data));
    
    if (decompressed_data.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (decompressed_data.size != input_data.size || 
        memcmp(input_data.data, decompressed_data.data, input_data.size) != 0) {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression successful - data verified\n");
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_data.size; i++) {
        if (isprint(decompressed_data.data[i])) {
            printf("%c", decompressed_data.data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
    
    return