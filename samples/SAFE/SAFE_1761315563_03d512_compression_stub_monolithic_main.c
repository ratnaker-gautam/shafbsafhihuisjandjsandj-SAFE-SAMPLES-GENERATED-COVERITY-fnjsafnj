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
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && input->data[i + count] == current) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        i += count;
    }
    
    output->size = out_idx;
    return 0;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0 || input->size % 2 != 0) {
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

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if (i < buf->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int read_input_string(char* str, size_t max_len) {
    if (str == NULL || max_len == 0) {
        return -1;
    }
    
    printf("Enter text to compress (max %zu characters): ", max_len - 1);
    
    if (fgets(str, (int)max_len, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
    
    return 0;
}

int main(void) {
    char input_str[MAX_INPUT_SIZE];
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (read_input_string(input_str, sizeof(input_str)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(original.data, input_str, input_len);
    original.size = input_len;
    
    printf("Original data (%zu bytes): ", original.size);
    print_buffer_hex(&original);
    
    if (compress_rle(&original, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(&compressed);
    
    if (decompress_rle(&compressed, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (decompressed.size != original.size || memcmp(original.data, decompressed.data, original.size) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / original.size) * 100.0);
    
    return 0;
}