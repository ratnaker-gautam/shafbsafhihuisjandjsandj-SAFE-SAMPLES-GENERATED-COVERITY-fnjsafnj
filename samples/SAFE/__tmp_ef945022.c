//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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
        
        for (; j < input_size && j < i + 255; j++) {
            if (input[j] == current) {
                count++;
            } else {
                break;
            }
        }
        
        output->data[output->size] = current;
        output->data[output->size + 1] = (uint8_t)count;
        output->size += 2;
        i += count;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    do {
        if (i >= input_size) break;
        
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        while (k < count) {
            output->data[output->size] = value;
            output->size++;
            k++;
        }
        
        i += 2;
    } while (i < input_size);
}

int main() {
    Buffer input_buf = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, input_len);
    input_buf.size = input_len;
    
    compress_run_length(input_buf.data, input_buf.size, &compressed);
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_buf.size - compressed.size) * 100.0f / input_buf.size);
        
        printf("Compressed data: ");
        for (size_t idx = 0; idx < compressed.size; idx += 2) {
            if (idx > 0) printf(" ");
            printf("%c:%d", compressed.data[idx], compressed.data[idx + 1]);
        }
        printf("\n");
        
        decompress_run_length(compressed.data, compressed.size, &decompressed);
        
        if (decompressed.size == input_buf.size && 
            memcmp(decompressed.data, input_buf.data, input_buf.size) == 0) {
            printf("Decompression successful: %.*s\n", (int)decompressed.size, decompressed.data);
        } else {
            printf("Decompression failed\n");
            return 1;
        }
    } else {
        printf("Compression failed\n");
        return 1;
    }
    
    return 0;
}