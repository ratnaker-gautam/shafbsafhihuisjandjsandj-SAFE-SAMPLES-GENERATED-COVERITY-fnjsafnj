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
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size % 2 != 0) {
        return -1;
    }
    
    size_t decompressed_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (decompressed_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_size++] = value;
        }
    }
    
    *output_size = decompressed_size;
    return 0;
}

int read_input(uint8_t* buffer, size_t* size) {
    if (buffer == NULL || size == NULL) {
        return -1;
    }
    
    printf("Enter text to compress (max %zu characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    *size = len;
    return 0;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 2 == 0) {
            printf(" ");
        }
    }
    if (buffer->size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    if (read_input(input, &input_size) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    
    if (compress_rle(input, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100.0);
    
    printf("Compressed data (hex):\n");
    print_buffer_hex(&compressed);
    
    if (decompress_rle(&compressed, decompressed, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_size != input_size || 
        memcmp(input, decompressed, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression successful. Original text: %s\n", decompressed);
    
    return 0;
}