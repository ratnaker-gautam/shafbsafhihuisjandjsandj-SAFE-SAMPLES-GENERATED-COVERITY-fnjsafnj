//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        
        i = j;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    for (; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output->data[output->size] = value;
            output->size++;
            k++;
        } while (k < count);
    }
}

int read_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return -1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    
    while (total_read < max_size - 1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        buffer[total_read] = (uint8_t)c;
        total_read++;
    }
    
    buffer[total_read] = '\0';
    return (int)total_read;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

void print_buffer_text(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        if (buffer->data[i] >= 32 && buffer->data[i] <= 126) {
            putchar(buffer->data[i]);
        } else {
            putchar('.');
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    int input_size = read_input(input_data, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("No input received or error reading input.\n");
        return 1;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);
    print_buffer_text((Buffer*)&input_data);
    
    compress_run_length(input_data, (size_t)input_size, &compressed);
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_text(&decompressed);
    
    if (decompressed.size == (size_t)input_size && 
        memcmp(input_data, decompressed.data, decompressed.size) == 0) {
        printf("\nCompression/Decompression successful!\n");
        double ratio = (double)compressed.size / (double)input_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("\nCompression/Decompression failed!\n");
        return 1;
    }
    
    return 0;
}