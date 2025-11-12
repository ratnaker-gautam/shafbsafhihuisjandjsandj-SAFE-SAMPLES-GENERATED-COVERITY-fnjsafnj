//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    if (input->size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && input->data[i + count] == current) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        i += count;
    }
    
    output->size = out_idx;
    return 0;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    if (input->size % 2 != 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (buf->size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    char temp_input[MAX_INPUT_SIZE + 1];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(temp_input);
    if (len > 0 && temp_input[len - 1] == '\n') {
        temp_input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input.data, temp_input, len);
    input.size = len;
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer_hex(&input);
    
    if (compress_rle(&input, &compressed) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input.size * 100.0f);
    
    if (decompress_rle(&compressed, &decompressed) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (input.size != decompressed.size) {
        printf("Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Decompression verified: OK\n");
    
    return 0;
}