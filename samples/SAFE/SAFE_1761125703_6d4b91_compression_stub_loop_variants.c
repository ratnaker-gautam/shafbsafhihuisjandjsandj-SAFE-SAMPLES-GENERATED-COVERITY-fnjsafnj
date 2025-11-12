//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    size_t j = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t k = i + 1;
        while (k < input_size && input[k] == current && count < 255) {
            count++;
            k++;
        }
        
        if (j + 2 > MAX_OUTPUT_SIZE) break;
        
        output->data[j++] = current;
        output->data[j++] = (uint8_t)count;
        i += count;
    }
    
    output->size = j;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while (input_size < MAX_INPUT_SIZE) {
        c = getchar();
        if (c == EOF || c == '\n') break;
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    simple_compress(input_data, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100);
    
    printf("Compressed data: ");
    for (size_t idx = 0; idx < compressed.size; idx++) {
        printf("%02X ", compressed.data[idx]);
    }
    printf("\n");
    
    printf("Decompressed: ");
    for (size_t pos = 0; pos < compressed.size; pos += 2) {
        if (pos + 1 >= compressed.size) break;
        uint8_t value = compressed.data[pos];
        uint8_t count = compressed.data[pos + 1];
        
        for (uint8_t run = 0; run < count; run++) {
            putchar(value);
        }
    }
    printf("\n");
    
    return 0;
}