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

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) {
        return 0;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) {
        return 0;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return 0;
    }
    
    if (input->size == 0 || input->size % 2 != 0) {
        return 0;
    }
    
    *output_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size] = value;
            (*output_size)++;
        }
    }
    
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if (i < buffer->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t decompressed_size;
    
    printf("Enter input string (max %d characters): ", MAX_INPUT_SIZE);
    
    char temp_input[MAX_INPUT_SIZE + 1];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        input_data[i] = (uint8_t)temp_input[i];
    }
    
    printf("Original: %s\n", temp_input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_rle(input_data, input_len, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed (hex): ");
    print_buffer_hex(&compressed);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (!decompress_rle(&compressed, output_data, &decompressed_size)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    output_data[decompressed_size] = '\0';
    printf("Decompressed: %s\n", output_data);
    printf("Decompressed size: %zu bytes\n", decompressed_size);
    
    if (decompressed_size != input_len || memcmp(input_data, output_data, input_len) != 0) {
        printf("Verification failed\n");
        return 1;
    }
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_len * 100.0f);
    
    return 0;
}