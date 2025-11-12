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
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
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
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    printf("RLE Compression/Decompression Tool\n");
    printf("Enter 'c' to compress or 'd' to decompress: ");
    
    char choice;
    if (scanf(" %c", &choice) != 1) {
        fprintf(stderr, "Error reading choice\n");
        return 1;
    }
    
    if (choice != 'c' && choice != 'd') {
        fprintf(stderr, "Invalid choice. Use 'c' or 'd'\n");
        return 1;
    }
    
    printf("Enter input data as hexadecimal bytes (max %d bytes):\n", MAX_INPUT_SIZE);
    
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    char hex[3];
    
    while (input_size < MAX_INPUT_SIZE) {
        if (scanf("%2s", hex) != 1) {
            break;
        }
        
        if (strlen(hex) != 2 || !isxdigit(hex[0]) || !isxdigit(hex[1])) {
            break;
        }
        
        unsigned int byte;
        if (sscanf(hex, "%02x", &byte) != 1) {
            break;
        }
        
        input[input_size++] = (uint8_t)byte;
        
        int next_char = getchar();
        if (next_char == '\n' || next_char == EOF) {
            break;
        }
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    Buffer output;
    
    if (choice == 'c') {
        if (compress_rle(input, input_size, &output) != 0) {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
        printf("Compressed data (%zu bytes):\n", output.size);
    } else {
        if (decompress_rle(input, input_size, &output) != 0) {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
        printf("Decompressed data (%zu bytes):\n", output.size);
    }
    
    print_hex(output.data, output.size);
    
    return 0;
}