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
    
    while (in_idx < input_len && out_idx < output_size - 2) {
        uint8_t current = input[in_idx];
        uint8_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= output_size) {
            return -1;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return (int)out_idx;
}

int compress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size == 0) {
        return -1;
    }
    
    if (output_size < input_len) {
        return -1;
    }
    
    output[0] = input[0];
    for (size_t i = 1; i < input_len; i++) {
        if (i >= output_size) {
            return -1;
        }
        int16_t delta = (int16_t)input[i] - (int16_t)input[i-1];
        output[i] = (uint8_t)(delta & 0xFF);
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
    
    printf("Enter data to compress (up to 256 bytes, hex digits only): ");
    
    char buffer[512];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *pos = buffer;
    while (*pos != '\0' && input_len < sizeof(input_data)) {
        while (*pos != '\0' && isspace((unsigned char)*pos)) {
            pos++;
        }
        
        if (*pos == '\0') {
            break;
        }
        
        if (!isxdigit((unsigned char)*pos)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        
        char hex[3] = {0};
        hex[0] = *pos++;
        if (*pos != '\0' && !isspace((unsigned char)*pos)) {
            hex[1] = *pos++;
        }
        
        unsigned int value;
        if (sscanf(hex, "%2x", &value) != 1) {
            fprintf(stderr, "Invalid hex value\n");
            return 1;
        }
        
        input_data[input_len++] = (uint8_t)value;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No valid input data\n");
        return 1;
    }
    
    printf("Select compression type:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: Delta\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice >= COMPRESS_TYPE_COUNT) {
        fprintf(stderr, "Invalid compression type\n");
        return 1;
    }
    
    int result_size;
    
    switch (choice) {
        case COMPRESS_NONE:
            result_size = compress_none(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_RLE:
            result_size = compress_rle(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_DELTA:
            result_size = compress_delta(input_data, input_len, output_data, sizeof(output_data));
            break;
        default:
            fprintf(stderr, "Unknown compression type\n");
            return 1;
    }
    
    if (result_size < 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %d bytes\n", result_size);
    printf("Compression ratio: %.2f%%\n", (float)result_size / input_len * 100.0f);
    
    printf("Compressed data (hex): ");
    for (int i = 0