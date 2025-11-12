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

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            output->size = 0;
            return;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
}

void decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input_size) {
        if (in_idx + 1 >= input_size) {
            output->size = 0;
            return;
        }
        
        uint8_t count = input[in_idx++];
        uint8_t value = input[in_idx++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            output->size = 0;
            return;
        }
        
        for (uint8_t k = 0; k < count; k++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
}

int get_user_input(uint8_t* buffer, size_t max_size) {
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, (int)max_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return (int)len;
}

void print_buffer_hex(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

void print_buffer_text(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        if (isprint(buffer->data[i])) {
            printf("%c", buffer->data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    int input_len = get_user_input(input_data, MAX_INPUT_SIZE);
    if (input_len <= 0) {
        printf("No input received or input too long.\n");
        return 1;
    }
    
    compress_rle(input_data, (size_t)input_len, &compressed);
    if (compressed.size == 0) {
        printf("Compression failed: output buffer too small.\n");
        return 1;
    }
    
    printf("Original size: %d bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - (int)compressed.size) / input_len * 100.0f);
    
    printf("Compressed data (hex): ");
    print_buffer_hex(&compressed);
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    if (decompressed.size == 0) {
        printf("Decompression failed: output buffer too small.\n");
        return 1;
    }
    
    printf("Decompressed text: ");
    print_buffer_text(&decompressed);
    
    if (decompressed.size == (size_t)input_len && 
        memcmp(input_data, decompressed.data, decompressed.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}