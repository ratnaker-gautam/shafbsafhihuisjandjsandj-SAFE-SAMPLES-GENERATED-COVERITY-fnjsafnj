//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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

void initialize_buffer(Buffer *buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int read_input(Buffer *input) {
    char temp[MAX_INPUT_SIZE + 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)temp[i])) {
            return 0;
        }
        input->data[i] = (uint8_t)temp[i];
    }
    input->length = len;
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        i = j;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer *input, Buffer *output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        if (i + 1 >= input->length) {
            output->length = 0;
            return;
        }
        
        uint8_t character = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (out_idx + count >= MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = character;
        }
        
        i += 2;
    }
    
    output->length = out_idx;
}

void display_buffer(const Buffer *buf) {
    for (size_t i = 0; i < buf->length; i++) {
        putchar(buf->data[i]);
    }
    putchar('\n');
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    initialize_buffer(&input);
    initialize_buffer(&compressed);
    initialize_buffer(&decompressed);
    
    printf("Enter text to compress: ");
    if (!read_input(&input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input.length == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    simple_compress(&input, &compressed);
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    simple_decompress(&compressed, &decompressed);
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Original: ");
    display_buffer(&input);
    
    printf("Compressed: ");
    for (size_t i = 0; i < compressed.length; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    printf("Decompressed: ");
    display_buffer(&decompressed);
    
    if (memcmp(input.data, decompressed.data, input.length) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    double ratio = (double)compressed.length / input.length * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}