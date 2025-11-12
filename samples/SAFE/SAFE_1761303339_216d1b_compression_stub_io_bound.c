//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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

int read_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) {
        return -1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    
    while ((c = getchar()) != EOF && c != '\n' && total_read < max_size - 1) {
        buffer[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0) {
        return -1;
    }
    
    *actual_size = total_read;
    return 0;
}

void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    printf("Data (%zu bytes): ", buffer->size);
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer compressed;
    Buffer decompressed;
    
    if (read_input(input_data, MAX_INPUT_SIZE, &input_size) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < input_size; i++) {
        printf("%c", input_data[i]);
    }
    printf("\n");
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed: ");
    print_buffer(&compressed);
    printf("Compression ratio: %.2f\n", (float)input_size / compressed.size);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size != input_size || memcmp(input_data, decompressed.data, input_size) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}