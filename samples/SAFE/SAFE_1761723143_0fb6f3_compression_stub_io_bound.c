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

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (i + 1 >= input->size) {
            return -1;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[(*output_size)++] = value;
        }
        
        i += 2;
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

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    char choice;
    int result;
    
    printf("Enter input data as hexadecimal bytes (max %d bytes):\n", MAX_INPUT_SIZE);
    
    char line[5];
    while (input_size < MAX_INPUT_SIZE && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        
        if (strlen(line) != 3 || !isxdigit(line[0]) || !isxdigit(line[1]) || line[2] != '\n') {
            printf("Invalid hex byte format. Use two hex digits per byte.\n");
            return 1;
        }
        
        unsigned int byte;
        if (sscanf(line, "%02x", &byte) != 1) {
            printf("Failed to parse hex byte.\n");
            return 1;
        }
        
        input_data[input_size++] = (uint8_t)byte;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("Choose operation (c=compress, d=decompress): ");
    if (scanf(" %c", &choice) != 1) {
        printf("Failed to read choice.\n");
        return 1;
    }
    
    if (choice == 'c' || choice == 'C') {
        result = compress_rle(input_data, input_size, &compressed);
        if (result != 0) {
            printf("Compression failed.\n");
            return 1;
        }
        
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_hex(compressed.data, compressed.size);
        
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_size - compressed.size) / input_size * 100.0f);
    }
    else if (choice == 'd' || choice == 'D') {
        Buffer temp;
        temp.size = input_size;
        memcpy(temp.data, input_data, input_size);
        
        size_t decompressed_size;
        result = decompress_rle(&temp, output_data, &decompressed_size);
        if (result != 0) {
            printf("Decompression failed.\n");
            return 1;
        }
        
        printf("Decompressed data (%zu bytes):\n", decompressed_size);
        print_hex(output_data, decompressed_size);
    }
    else {
        printf("Invalid choice.\n");
        return 1;
    }
    
    return 0;
}