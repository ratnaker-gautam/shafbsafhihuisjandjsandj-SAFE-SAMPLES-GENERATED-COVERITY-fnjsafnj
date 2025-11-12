//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    if (input->size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
    return 0;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    if (input->size % 2 != 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 0;
}

int read_input(Buffer* buf) {
    if (buf == NULL) {
        return -1;
    }
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_SIZE) {
        return -1;
    }
    
    buf->size = len;
    memcpy(buf->data, line, len);
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) {
        printf("(empty)\n");
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

void print_buffer_text(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) {
        printf("(empty)\n");
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        if (isprint(buf->data[i])) {
            printf("%c", buf->data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (read_input(&input) != 0) {
        printf("Error reading input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    printf("Hex: ");
    print_buffer_hex(&input);
    printf("Text: ");
    print_buffer_text(&input);
    
    if (compress_rle(&input, &compressed) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    printf("Hex: ");
    print_buffer_hex(&compressed);
    
    if (decompress_rle(&compressed, &decompressed) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    printf("Hex: ");
    print_buffer_hex(&decompressed);
    printf("Text: ");
    print_buffer_text(&decompressed);
    
    if (input.size != decompressed.size || 
        memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Verification failed: decompressed data doesn't match original\n");
        return 1;
    }
    
    printf("\nCompression successful. Ratio: %.2f%%\n", 
           (double)compressed.size / input.size * 100);
    
    return 0;
}