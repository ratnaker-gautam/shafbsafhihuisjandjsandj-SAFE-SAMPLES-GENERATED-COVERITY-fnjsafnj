//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_DELTA,
    COMPRESS_TYPE_COUNT
};

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size == 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        if (out_idx + 2 > output_size) {
            return -1;
        }
        
        uint8_t current = input[in_idx];
        uint8_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return (int)out_idx;
}

int compress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size < input_len) {
        return -1;
    }
    
    output[0] = input[0];
    
    for (size_t i = 1; i < input_len; i++) {
        output[i] = input[i] - input[i-1];
    }
    
    return (int)input_len;
}

int compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size < input_len) {
        return -1;
    }
    
    memcpy(output, input, input_len);
    return (int)input_len;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    char input_buffer[1024];
    
    printf("Enter data to compress (max 255 bytes): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t buffer_len = strlen(input_buffer);
    if (buffer_len > 0 && input_buffer[buffer_len-1] == '\n') {
        input_buffer[--buffer_len] = '\0';
    }
    
    if (buffer_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    input_len = buffer_len;
    if (input_len > 255) {
        input_len = 255;
    }
    
    memcpy(input_data, input_buffer, input_len);
    
    printf("Select compression type:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: Delta\n");
    printf("Choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice >= COMPRESS_TYPE_COUNT) {
        fprintf(stderr, "Invalid compression type\n");
        return 1;
    }
    
    int result_len;
    
    switch (choice) {
        case COMPRESS_NONE:
            result_len = compress_none(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_RLE:
            result_len = compress_rle(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_DELTA:
            result_len = compress_delta(input_data, input_len, output_data, sizeof(output_data));
            break;
        default:
            fprintf(stderr, "Unknown compression type\n");
            return 1;
    }
    
    if (result_len < 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %d bytes\n", result_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result_len / input_len) * 100);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result_len; i++) {
        printf("%02x ", output_data[i]);
    }
    printf("\n");
    
    return 0;
}