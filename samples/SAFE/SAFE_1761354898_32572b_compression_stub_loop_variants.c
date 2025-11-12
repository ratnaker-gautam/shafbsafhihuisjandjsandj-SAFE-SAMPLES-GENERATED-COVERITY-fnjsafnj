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

int validate_input(const uint8_t *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
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

void simple_decompress(const Buffer *input, Buffer *output) {
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

void print_buffer_hex(const Buffer *buf) {
    for (size_t idx = 0; idx < buf->length; idx++) {
        printf("%02X", buf->data[idx]);
    }
    printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    memcpy(input.data, temp, len);
    input.length = len;
    
    if (!validate_input(input.data, input.length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    simple_compress(&input, &compressed);
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    simple_decompress(&compressed, &decompressed);
    if (decompressed.length == 0 || decompressed.length != input.length) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input.data, decompressed.data, input.length) != 0) {
        fprintf(stderr, "Data mismatch\n");
        return 1;
    }
    
    printf("Original: ");
    print_buffer_hex(&input);
    printf("Compressed: ");
    print_buffer_hex(&compressed);
    printf("Decompressed: ");
    print_buffer_hex(&decompressed);
    
    double ratio = (double)compressed.length / input.length * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}