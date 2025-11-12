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
        
        if (count > 3 || current < 32 || current > 126) {
            if (output->length + 3 > MAX_OUTPUT_SIZE) break;
            output->data[output->length++] = 0xFF;
            output->data[output->length++] = count;
            output->data[output->length++] = current;
        } else {
            if (output->length + count > MAX_OUTPUT_SIZE) break;
            for (size_t k = 0; k < count; k++) {
                output->data[output->length++] = current;
            }
        }
        
        i = j;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->length = 0;
    
    while (i < input->length) {
        if (input->data[i] == 0xFF && i + 2 < input->length) {
            size_t count = input->data[i + 1];
            uint8_t value = input->data[i + 2];
            
            if (output->length + count > MAX_OUTPUT_SIZE) break;
            
            for (size_t j = 0; j < count; j++) {
                output->data[output->length++] = value;
            }
            i += 3;
        } else {
            if (output->length >= MAX_OUTPUT_SIZE) break;
            output->data[output->length++] = input->data[i];
            i++;
        }
    }
}

int read_input(Buffer* buf) {
    char temp[MAX_INPUT_SIZE * 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    buf->length = len;
    for (size_t i = 0; i < len; i++) {
        buf->data[i] = (uint8_t)temp[i];
    }
    return 1;
}

void print_buffer(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        putchar(buf->data[i]);
    }
    putchar('\n');
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    printf("Enter text to compress: ");
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_input(input.data, input.length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    simple_compress(&input, &compressed);
    simple_decompress(&compressed, &decompressed);
    
    printf("Original length: %zu\n", input.length);
    printf("Compressed length: %zu\n", compressed.length);
    printf("Decompressed length: %zu\n", decompressed.length);
    
    if (input.length == decompressed.length && 
        memcmp(input.data, decompressed.data, input.length) == 0) {
        printf("Compression successful: ");
        print_buffer(&compressed);
    } else {
        printf("Compression failed\n");
        return 1;
    }
    
    return 0;
}