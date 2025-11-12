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
        
        output->data[output->size] = current;
        output->data[output->size + 1] = (uint8_t)count;
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
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
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

int validate_input(const char* input, size_t* length) {
    if (input == NULL || length == NULL) {
        return 0;
    }
    
    size_t len = 0;
    while (input[len] != '\0') {
        len++;
        if (len > MAX_INPUT_SIZE) {
            return 0;
        }
    }
    
    *length = len;
    return 1;
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1] = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = 0;
    if (!validate_input(input_str, &input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    compress_run_length((uint8_t*)input_str, input_len, &compressed);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu\n", input_len);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100.0);
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size != input_len || 
        memcmp(input_str, decompressed.data, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i += 2) {
        if (i > 0) printf(" ");
        printf("%c:%d", compressed.data[i], compressed.data[i + 1]);
    }
    printf("\n");
    
    return 0;
}