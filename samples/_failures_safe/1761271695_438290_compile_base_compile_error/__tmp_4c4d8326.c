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

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if (i + 1 < buf->size) {
            printf(" ");
        }
    }
    if (buf->size % 16 != 0) {
        printf("\n");
    }
}

int hex_char_to_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int parse_hex_string(const char* str, Buffer* buf) {
    if (str == NULL || buf == NULL) return -1;
    
    size_t len = strlen(str);
    if (len == 0 || len % 2 != 0) return -1;
    
    buf->size = 0;
    
    for (size_t i = 0; i < len; i += 2) {
        if (buf->size >= MAX_INPUT_SIZE) return -1;
        
        int high = hex_char_to_value(str[i]);
        int low = hex_char_to_value(str[i + 1]);
        
        if (high < 0 || low < 0) return -1;
        
        buf->data[buf->size++] = (uint8_t)((high << 4) | low);
    }
    
    return 0;
}

int main(void) {
    char input_str[MAX_INPUT_SIZE * 2 + 1];
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    printf("Enter hex data to compress (e.g., 414141424242): ");
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
    }
    
    if (parse_hex_string(input_str, &input_buf) != 0) {
        fprintf(stderr, "Invalid hex input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_buf.size);
    print_buffer_hex(&input_buf);
    
    if (compress_rle(&input_buf, &compressed_buf) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_buf.size);
    print_buffer_hex(&compressed_buf);
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed_buf.size * 100.0f / (float)input_buf.size);
    
    if (decompress_rle(&compressed_buf, &decompressed_buf