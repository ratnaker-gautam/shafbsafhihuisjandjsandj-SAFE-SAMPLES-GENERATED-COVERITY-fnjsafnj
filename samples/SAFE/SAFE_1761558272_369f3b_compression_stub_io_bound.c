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
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }
    }
    
    return 0;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 2 == 0) {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    printf("Simple RLE Compression Tool\n");
    printf("Enter 'c' to compress or 'd' to decompress: ");
    
    char choice;
    if (scanf(" %c", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (choice != 'c' && choice != 'd') {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    printf("Enter data (hex characters only, max %d bytes): ", MAX_INPUT_SIZE / 2);
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (scanf("%s", hex_input) != 1) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex input length\n");
        return 1;
    }
    
    for (size_t i = 0; i < hex_len; i++) {
        if (!isxdigit(hex_input[i])) {
            fprintf(stderr, "Invalid hex character\n");
            return 1;
        }
    }
    
    size_t input_size = hex_len / 2;
    uint8_t input_data[MAX_INPUT_SIZE];
    
    for (size_t i = 0; i < input_size; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        input_data[i] = (uint8_t)strtoul(hex_byte, NULL, 16);
    }
    
    Buffer output;
    int result;
    
    if (choice == 'c') {
        result = compress_rle(input_data, input_size, &output);
        if (result != 0) {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
        printf("Compressed data (%zu bytes):\n", output.size);
    } else {
        result = decompress_rle(input_data, input_size, &output);
        if (result != 0) {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
        printf("Decompressed data (%zu bytes):\n", output.size);
    }
    
    print_hex(output.data, output.size);
    
    return 0;
}