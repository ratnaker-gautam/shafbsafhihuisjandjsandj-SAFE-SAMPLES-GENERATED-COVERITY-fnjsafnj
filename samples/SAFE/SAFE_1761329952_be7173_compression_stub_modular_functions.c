//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_index + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[out_index++] = (uint8_t)count;
        output->data[out_index++] = current;
        i += count;
    }
    
    output->size = out_index;
    return out_index;
}

size_t simple_decompress(const CompressedData* compressed, uint8_t* output, size_t max_output_size) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0 || compressed->size % 2 != 0) return 0;
    
    size_t out_index = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (out_index + count > max_output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_index++] = value;
        }
    }
    
    return out_index;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int get_user_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL) return 0;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    char temp_buffer[MAX_INPUT_SIZE];
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) == NULL) return 0;
    
    size_t len = strlen(temp_buffer);
    if (len > 0 && temp_buffer[len - 1] == '\n') {
        temp_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= max_size) return 0;
    
    memcpy(buffer, temp_buffer, len);
    buffer[len] = '\0';
    
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    if (!get_user_input(input_buffer, sizeof(input_buffer))) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (!validate_input((char*)input_buffer, input_len)) {
        printf("Error: Input validation failed\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_len);
    print_hex(input_buffer, input_len);
    
    memset(&compressed, 0, sizeof(compressed));
    size_t compressed_size = simple_compress(input_buffer, input_len, &compressed);
    
    if (compressed_size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)compressed_size / input_len * 100.0f);
    
    memset(decompressed_buffer, 0, sizeof(decompressed_buffer));
    size_t decompressed_size = simple_decompress(&compressed, decompressed_buffer, 
                                                sizeof(decompressed_buffer));
    
    if (decompressed_size != input_len || 
        memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Error: Decompression verification failed\n");
        return 1;
    }
    
    printf("\nDecompressed successfully (%zu bytes):\n", decompressed_size);
    printf("%s\n", decompressed_buffer);
    
    return 0;
}