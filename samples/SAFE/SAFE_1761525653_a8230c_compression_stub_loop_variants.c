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

void compress_rle(const Buffer* input, Buffer* output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i = j;
    }
    
    output->length = out_idx;
}

void decompress_rle(const Buffer* input, Buffer* output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    for (; i < input->length; i += 2) {
        if (i + 1 >= input->length) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output->data[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    output->length = out_idx;
}

int get_user_input(Buffer* buf) {
    char temp[MAX_INPUT_SIZE * 2];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)temp[i]) && temp[i] != ' ') {
            return 0;
        }
    }
    
    memcpy(buf->data, temp, len);
    buf->length = len;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!get_user_input(&input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input.length);
    for (size_t i = 0; i < input.length; i++) {
        printf("%c", input.data[i]);
    }
    printf("\n");
    
    compress_rle(&input, &compressed);
    printf("Compressed (%zu bytes): ", compressed.length);
    print_buffer_hex(&compressed);
    
    decompress_rle(&compressed, &decompressed);
    printf("Decompressed (%zu bytes): ", decompressed.length);
    for (size_t i = 0; i < decompressed.length; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (input.length == decompressed.length && 
        memcmp(input.data, decompressed.data, input.length) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Compression/Decompression failed\n");
        return 1;
    }
    
    return 0;
}