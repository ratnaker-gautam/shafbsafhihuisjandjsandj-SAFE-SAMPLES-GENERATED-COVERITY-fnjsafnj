//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
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

int read_input(Buffer* buffer) {
    if (buffer == NULL) {
        return -1;
    }
    
    buffer->size = 0;
    int c;
    
    while ((c = getchar()) != EOF && buffer->size < MAX_INPUT_SIZE) {
        buffer->data[buffer->size++] = (uint8_t)c;
    }
    
    if (buffer->size == 0) {
        return -1;
    }
    
    return 0;
}

void write_output(const Buffer* buffer) {
    if (buffer == NULL || buffer->size == 0) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        putchar(buffer->data[i]);
    }
}

int main(void) {
    printf("Enter 'c' to compress or 'd' to decompress: ");
    int choice = getchar();
    if (choice != 'c' && choice != 'd') {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    Buffer input_buffer;
    if (read_input(&input_buffer) != 0) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    Buffer output_buffer;
    int result;
    
    if (choice == 'c') {
        result = compress_rle(input_buffer.data, input_buffer.size, &output_buffer);
        if (result != 0) {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
        printf("Compressed data (%zu bytes):\n", output_buffer.size);
    } else {
        result = decompress_rle(input_buffer.data, input_buffer.size, &output_buffer);
        if (result != 0) {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
        printf("Decompressed data (%zu bytes):\n", output_buffer.size);
    }
    
    write_output(&output_buffer);
    return 0;
}