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

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count >= 3) {
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
            i = j;
        } else {
            for (size_t k = 0; k < count && out_idx < MAX_OUTPUT_SIZE; k++) {
                output->data[out_idx++] = current;
            }
            i += count;
        }
    }
    
    output->size = out_idx;
}

void decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count >= MAX_OUTPUT_SIZE) {
                break;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output->data[out_idx++] = value;
            }
            i += 3;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) {
                break;
            }
            output->data[out_idx++] = input[i++];
        }
    }
    
    output->size = out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    compress_rle(input_data, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_len - compressed.size) * 100.0f / input_len);
        
        decompress_rle(compressed.data, compressed.size, &decompressed);
        
        if (decompressed.size == input_len && 
            memcmp(input_data, decompressed.data, input_len) == 0) {
            printf("Decompression verified: OK\n");
        } else {
            printf("Decompression verification failed\n");
            return 1;
        }
    }
    
    return 0;
}