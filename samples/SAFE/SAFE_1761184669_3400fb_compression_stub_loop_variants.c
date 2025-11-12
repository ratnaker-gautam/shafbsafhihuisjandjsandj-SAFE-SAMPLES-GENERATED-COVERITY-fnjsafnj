//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

void compress_run_length(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->size = 0;
    
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        for (size_t j = i + 1; j < input->size; j++) {
            if (input->data[j] == current && count < 255) {
                count++;
            } else {
                break;
            }
        }
        
        if (output->size + 2 <= MAX_INPUT_SIZE) {
            output->data[output->size++] = (uint8_t)count;
            output->data[output->size++] = current;
        } else {
            break;
        }
        
        i += count;
    }
}

void decompress_run_length(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->size = 0;
    
    while (i < input->size) {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (output->size < MAX_INPUT_SIZE) {
                output->data[output->size++] = value;
            } else {
                break;
            }
        }
        
        i += 2;
    }
}

int read_input(Buffer* buffer) {
    char temp[MAX_INPUT_SIZE * 2];
    
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return 0;
    }
    
    buffer->size = 0;
    for (size_t i = 0; i < len && buffer->size < MAX_INPUT_SIZE; i++) {
        if (isprint((unsigned char)temp[i])) {
            buffer->data[buffer->size++] = (uint8_t)temp[i];
        }
    }
    
    return buffer->size > 0;
}

void print_buffer(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        putchar(buffer->data[i]);
    }
    putchar('\n');
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    printf("Enter text to compress: ");
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    compress_run_length(&input, &compressed);
    decompress_run_length(&compressed, &decompressed);
    
    printf("Original: ");
    print_buffer(&input);
    
    printf("Compressed: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%u ", compressed.data[i]);
    }
    putchar('\n');
    
    printf("Decompressed: ");
    print_buffer(&decompressed);
    
    if (memcmp(input.data, decompressed.data, input.size) == 0 && input.size == decompressed.size) {
        printf("Verification: Success\n");
    } else {
        printf("Verification: Failed\n");
        return 1;
    }
    
    double ratio = (double)compressed.size / input.size * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}