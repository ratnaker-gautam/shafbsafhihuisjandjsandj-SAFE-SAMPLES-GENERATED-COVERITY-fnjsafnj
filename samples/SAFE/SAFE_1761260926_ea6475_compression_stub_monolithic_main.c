//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 1 || current == 0xFF) {
            output->data[output->size++] = 0xFF;
            output->data[output->size++] = current;
            output->data[output->size++] = (uint8_t)count;
        } else {
            output->data[output->size++] = current;
        }
        
        i += count;
    }
}

void decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            for (uint8_t j = 0; j < count && output->size < MAX_OUTPUT_SIZE; j++) {
                output->data[output->size++] = value;
            }
            
            i += 3;
        } else {
            output->data[output->size++] = input[i];
            i++;
        }
    }
}

int validate_input(const char* input, size_t* length) {
    if (input == NULL || length == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
    }
    
    *length = len;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    
    if (buffer->size > 0) {
        printf("\n");
    }
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_length = 0;
    if (!validate_input(input, &input_length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input_length > 0 && input[input_length - 1] == '\n') {
        input[input_length - 1] = '\0';
        input_length--;
    }
    
    if (input_length == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    compress_rle((uint8_t*)input, input_length, &compressed);
    
    printf("\nOriginal size: %zu bytes\n", input_length);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           compressed.size > 0 ? (100.0 * compressed.size / input_length) : 0.0);
    
    printf("\nCompressed data (hex):\n");
    print_buffer_hex(&compressed);
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size == input_length && 
        memcmp(decompressed.data, input, input_length) == 0) {
        printf("\nDecompression successful\n");
        printf("Decompressed text: %.*s\n", (int)decompressed.size, decompressed.data);
    } else {
        fprintf(stderr, "\nDecompression failed\n");
        return 1;
    }
    
    return 0;
}