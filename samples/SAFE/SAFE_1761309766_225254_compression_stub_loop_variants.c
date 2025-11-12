//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->length = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        size_t j = i + 1;
        
        for (; j < input->length; j++) {
            if (input->data[j] == current) {
                count++;
            } else {
                break;
            }
        }
        
        if (output->length + 2 > MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        output->data[output->length++] = current;
        output->data[output->length++] = (uint8_t)(count > 255 ? 255 : count);
        
        i = j;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->length = 0;
    
    while (i < input->length) {
        if (i + 1 >= input->length) {
            output->length = 0;
            return;
        }
        
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (output->length + count > MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        size_t k = 0;
        do {
            output->data[output->length++] = value;
            k++;
        } while (k < count);
        
        i += 2;
    }
}

int main() {
    Buffer input, compressed, decompressed;
    char line[MAX_INPUT_SIZE * 2];
    
    printf("Enter text to compress: ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    input.length = len;
    for (size_t i = 0; i < len; i++) {
        input.data[i] = (uint8_t)line[i];
    }
    
    if (!validate_input(input.data, input.length)) {
        printf("Invalid input\n");
        return 1;
    }
    
    simple_compress(&input, &compressed);
    if (compressed.length == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.length);
    printf("Original size: %zu bytes\n", input.length);
    
    if (compressed.length < input.length) {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.length / input.length) * 100.0);
    } else {
        printf("No compression achieved\n");
    }
    
    simple_decompress(&compressed, &decompressed);
    if (decompressed.length == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (decompressed.length == input.length && 
        memcmp(decompressed.data, input.data, input.length) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    return 0;
}