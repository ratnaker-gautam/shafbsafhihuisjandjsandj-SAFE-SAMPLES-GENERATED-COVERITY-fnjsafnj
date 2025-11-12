//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (!validate_input(input->data, input->size)) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
        
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
    return 1;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (!validate_input(input->data, input->size)) return 0;
    if (input->size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int hex_char_to_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int parse_hex_input(const char* str, Buffer* buf) {
    if (str == NULL || buf == NULL) return 0;
    
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_SIZE * 2) return 0;
    if (len % 2 != 0) return 0;
    
    buf->size = 0;
    for (size_t i = 0; i < len; i += 2) {
        int high = hex_char_to_value(str[i]);
        int low = hex_char_to_value(str[i + 1]);
        
        if (high < 0 || low < 0) return 0;
        
        buf->data[buf->size++] = (uint8_t)((high << 4) | low);
    }
    
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1] = {0};
    
    printf("Enter hex data (e.g., 414141424242): ");
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(hex_input);
    if (len > 0 && hex_input[len - 1] == '\n') {
        hex_input[len - 1] = '\0';
    }
    
    for (size_t i = 0; hex_input[i] != '\0'; i++) {
        if (hex_input[i] != ' ' && !isxdigit(hex_input[i])) {
            printf("Invalid hex character\n");
            return 1;
        }
    }
    
    char cleaned_hex[MAX_INPUT_SIZE * 2 + 1] = {0};
    size_t clean_idx = 0;
    for (size_t i = 0; hex_input[i] != '\0' && clean_idx < sizeof(cleaned_hex) - 1; i++) {
        if (hex_input[i] != ' ') {
            cleaned_hex[clean_idx++] = hex_input[i];
        }
    }
    
    if (!parse_hex_input(cleaned_