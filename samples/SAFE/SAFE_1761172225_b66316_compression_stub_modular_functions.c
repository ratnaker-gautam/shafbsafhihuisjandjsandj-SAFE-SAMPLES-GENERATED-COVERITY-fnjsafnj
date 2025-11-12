//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer result = {0};
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return result;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size && output_index < MAX_OUTPUT_SIZE - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (output_index + 2 >= MAX_OUTPUT_SIZE) break;
            result.data[output_index++] = 0xFF;
            result.data[output_index++] = current;
            result.data[output_index++] = (uint8_t)count;
            i += count;
        } else {
            if (output_index >= MAX_OUTPUT_SIZE) break;
            result.data[output_index++] = current;
            i++;
        }
    }
    
    result.size = output_index;
    return result;
}

Buffer decompress_data(const uint8_t* input, size_t input_size) {
    Buffer result = {0};
    if (input == NULL || input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return result;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size && output_index < MAX_OUTPUT_SIZE; i++) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output_index + count > MAX_OUTPUT_SIZE) {
                break;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                result.data[output_index++] = value;
            }
            i += 2;
        } else {
            result.data[output_index++] = input[i];
        }
    }
    
    result.size = output_index;
    return result;
}

int validate_input(const char* input, size_t max_len) {
    if (input == NULL) return 0;
    for (size_t i = 0; i < max_len && input[i] != '\0'; i++) {
        if (input[i] < 0 || input[i] > 127) return 0;
    }
    return 1;
}

void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_str, input_len)) {
        fprintf(stderr, "Invalid input characters\n");
        return 1;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_len);
    for (size_t i = 0; i < input_len; i++) {
        printf("%02X ", (uint8_t)input_str[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (input_len % 16 != 0) printf("\n");
    
    Buffer compressed = compress_data((uint8_t*)input_str, input_len);
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer(&compressed);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    printf("Decompressed data (%zu bytes):\n", decompressed.size);
    print_buffer(&decompressed);
    
    if (decompressed.size == input_len && 
        memcmp(decompressed.data, input_str, input_len) == 0) {
        printf("Compression/Decompression successful\n");
        float ratio = (float)compressed.size / input_len * 100.0f;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("Compression/Decompression failed\n");
        return 1;
    }
    
    return 0;
}