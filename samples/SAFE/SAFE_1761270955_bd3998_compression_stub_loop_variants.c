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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    input.length = len;
    for (size_t i = 0; i < len; i++) {
        input.data[i] = (uint8_t)line[i];
    }
    
    if (!validate_input(input.data, input.length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    simple_compress(&input, &compressed);
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.length);
    for (size_t i = 0; i < compressed.length; i++) {
        if (isprint(compressed.data[i])) {
            printf("%c", compressed.data[i]);
        } else {
            printf("[%d]", compressed.data[i]);
        }
    }
    printf("\n");
    
    simple_decompress(&compressed, &decompressed);
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.length);
    for (size_t i = 0; i < decompressed.length; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.length != input.length || 
        memcmp(input.data, decompressed.data, input.length) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.length / input.length * 100.0f);
    
    return 0;
}