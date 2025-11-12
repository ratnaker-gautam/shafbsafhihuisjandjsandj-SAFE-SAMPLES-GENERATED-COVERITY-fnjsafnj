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
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        if (i + 1 >= input->size) {
            return -1;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > *output_size) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    *output_size = out_idx;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    size_t decompressed_size;
    int choice;
    
    printf("Enter 1 to compress sample data, 2 to decompress: ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (choice == 1) {
        printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
        getchar();
        
        int c;
        while (input_size < MAX_INPUT_SIZE - 1 && (c = getchar()) != '\n' && c != EOF) {
            if (isprint(c)) {
                input_data[input_size++] = (uint8_t)c;
            }
        }
        input_data[input_size] = '\0';
        
        if (input_size == 0) {
            strcpy((char*)input_data, "AAAABBBCCDAA");
            input_size = strlen((char*)input_data);
            printf("Using default sample: %s\n", input_data);
        }
        
        if (compress_rle(input_data, input_size, &compressed) != 0) {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
        
        printf("Compressed data (%zu bytes): ", compressed.size);
        for (size_t i = 0; i < compressed.size; i++) {
            printf("%02X ", compressed.data[i]);
        }
        printf("\n");
        
        decompressed_size = MAX_INPUT_SIZE;
        if (decompress_rle(&compressed, output_data, &decompressed_size) != 0) {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
        
        output_data[decompressed_size] = '\0';
        printf("Decompressed: %s\n", output_data);
        
    } else if (choice == 2) {
        printf("Enter compressed data as hex pairs (e.g., 04 41 03 42): ");
        
        compressed.size = 0;
        unsigned int byte;
        while (compressed.size < MAX_OUTPUT_SIZE && scanf("%02X", &byte) == 1) {
            if (byte > 255) {
                fprintf(stderr, "Invalid byte value\n");
                return 1;
            }
            compressed.data[compressed.size++] = (uint8_t)byte;
        }
        
        if (compressed.size % 2 != 0) {
            fprintf(stderr, "Invalid compressed data format\n");
            return 1;
        }
        
        decompressed_size = MAX_INPUT_SIZE;
        if (decompress_rle(&compressed, output_data, &decompressed_size) != 0) {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
        
        output_data[decompressed_size] = '\0';
        printf("Decompressed: %s\n", output_data);
        
    } else {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    return 0;
}