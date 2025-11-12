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
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= output_size) break;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size) {
        if (in_idx + 1 >= input_size) break;
        
        uint8_t value = input[in_idx++];
        uint8_t count = input[in_idx++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t i = 0; i < count && out_idx < output_size; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char input[MAX_INPUT_SIZE + 1];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    buffer->size = len;
    memcpy(buffer->data, input, len);
    return 1;
}

void display_buffer(const char* label, const uint8_t* data, size_t size) {
    if (label == NULL || data == NULL) return;
    
    printf("%s (%zu bytes): ", label, size);
    for (size_t i = 0; i < size && i < 32; i++) {
        if (isprint(data[i])) {
            printf("%c", data[i]);
        } else {
            printf("\\x%02x", data[i]);
        }
    }
    printf("\n");
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!get_user_input(&input_data)) {
        printf("Error: Failed to read input\n");
        return 1;
    }
    
    if (!validate_input(input_data.data, input_data.size)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    compressed.size = compress_rle(input_data.data, input_data.size, 
                                 compressed.data, sizeof(compressed.data));
    
    if (compressed.size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    decompressed.size = decompress_rle(compressed.data, compressed.size,
                                     decompressed.data, sizeof(decompressed.data));
    
    if (decompressed.size == 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    if (decompressed.size != input_data.size || 
        memcmp(input_data.data, decompressed.data, input_data.size) != 0) {
        printf("Error: Compression/decompression mismatch\n");
        return 1;
    }
    
    display_buffer("Original", input_data.data, input_data.size);
    display_buffer("Compressed", compressed.data, compressed.size);
    display_buffer("Decompressed", decompressed.data, decompressed.size);
    
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_data.size) * 100.0);
    
    return 0;
}