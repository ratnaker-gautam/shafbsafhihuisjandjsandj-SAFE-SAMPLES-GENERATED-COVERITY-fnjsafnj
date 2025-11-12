//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
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
        if (output->size >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        
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

int get_user_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) {
        return -1;
    }
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, (int)max_size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    *actual_size = len;
    return 0;
}

void print_buffer_hex(const Buffer* buffer) {
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
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (get_user_input(input_buffer, MAX_INPUT_SIZE, &input_size) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original text: %s\n", input_buffer);
    printf("Original size: %zu bytes\n", input_size);
    
    if (compress_rle(input_buffer, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) * 100.0f / (float)input_size);
    print_buffer_hex(&compressed);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed.size != input_size || 
        memcmp(input_buffer, decompressed.data, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed text: %s\n", decompressed.data);
    printf("Decompression successful - data verified\n");
    
    return 0;
}