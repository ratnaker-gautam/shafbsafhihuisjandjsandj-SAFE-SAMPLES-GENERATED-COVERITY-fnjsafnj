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
        printf("%02X", data[i]);
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

int main(void) {
    printf("Simple RLE Compression Tool\n");
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
    
    printf("Enter data as hexadecimal bytes (e.g., 41 42 43): ");
    
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    char hex[3];
    
    while (scanf("%2s", hex) == 1) {
        if (input_size >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            return 1;
        }
        
        if (strlen(hex) != 2 || !isxdigit(hex[0]) || !isxdigit(hex[1])) {
            fprintf(stderr, "Invalid hex input\n");
            return 1;
        }
        
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        if (endptr == hex || *endptr != '\0' || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex byte\n");
            return 1;
        }
        
        input_data[input_size++] = (uint8_t)value;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No input data provided\n");
        return 1;
    }
    
    Buffer output_buffer;
    
    if (choice == 'c') {
        if (compress_rle(input_data, input_size, &output_buffer) != 0) {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
        printf("Compressed data (%zu bytes):\n", output_buffer.size);
    } else {
        if (decompress_rle(input_data, input_size, &output_buffer) != 0) {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
        printf("Decompressed data (%zu bytes):\n", output_buffer.size);
    }
    
    print_hex(output_buffer.data, output_buffer.size);
    
    return 0;
}