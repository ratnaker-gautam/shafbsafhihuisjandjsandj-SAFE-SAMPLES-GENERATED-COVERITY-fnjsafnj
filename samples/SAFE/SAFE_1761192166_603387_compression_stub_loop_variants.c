//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
        while (j < input_size && j - i < 255 && input[j] == current) {
            count++;
            j++;
        }
        
        output->data[output->size] = (uint8_t)count;
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
            output->data[output->size + k] = value;
            k++;
        } while (k < count);
        
        output->size += count;
    }
}

int is_printable(const uint8_t* data, size_t size) {
    size_t i = 0;
    while (i < size) {
        if (!isprint(data[i]) && data[i] != '\n' && data[i] != '\t' && data[i] != '\r') {
            return 0;
        }
        i++;
    }
    return 1;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer compressed;
    Buffer decompressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input);
    if (input_size > 0 && input[input_size - 1] == '\n') {
        input[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    compress_run_length(input, input_size, &compressed);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size < input_size) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_size) * 100);
    } else {
        printf("No compression achieved\n");
    }
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size != input_size) {
        fprintf(stderr, "Decompression size mismatch\n");
        return 1;
    }
    
    if (memcmp(input, decompressed.data, input_size) != 0) {
        fprintf(stderr, "Decompression data mismatch\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    
    if (is_printable(decompressed.data, decompressed.size)) {
        printf("Decompressed text: ");
        size_t idx = 0;
        while (idx < decompressed.size) {
            putchar(decompressed.data[idx]);
            idx++;
        }
        printf("\n");
    }
    
    return 0;
}