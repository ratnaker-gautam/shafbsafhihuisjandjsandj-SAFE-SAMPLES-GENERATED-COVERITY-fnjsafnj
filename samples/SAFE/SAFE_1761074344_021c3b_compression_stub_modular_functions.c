//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len && output_len + 2 <= output_max) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[output_len++] = (uint8_t)count;
        output[output_len++] = current;
        i += count;
    }
    
    if (i < input_len) {
        return 0;
    }
    
    return output_len;
}

size_t decompress_rle(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0 || input_len % 2 != 0) {
        return 0;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len && output_len < output_max) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (output_len + count > output_max) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_len++] = value;
        }
    }
    
    return output_len;
}

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) {
        return 0;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE + 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_SIZE) {
        return 0;
    }
    
    memcpy(buffer->data, temp, len);
    buffer->length = len;
    
    return 1;
}

void display_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    if (!get_user_input(&input_buffer)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buffer.length);
    display_hex(input_buffer.data, input_buffer.length);
    
    size_t compressed_size = compress_rle(input_buffer.data, input_buffer.length, 
                                        compressed_buffer.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    compressed_buffer.length = compressed_size;
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    display_hex(compressed_buffer.data, compressed_size);
    
    size_t decompressed_size = decompress_rle(compressed_buffer.data, compressed_buffer.length,
                                            decompressed_buffer.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size != input_buffer.length || 
        memcmp(input_buffer.data, decompressed_buffer.data, input_buffer.length) != 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    display_hex(decompressed_buffer.data, decompressed_size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / input_buffer.length) * 100.0);
    
    return 0;
}