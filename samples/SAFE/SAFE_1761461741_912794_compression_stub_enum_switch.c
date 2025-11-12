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

int rle_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_len) {
            return -1;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return (int)out_idx;
}

int delta_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return -1;
    }
    
    if (output_len < input_len) {
        return -1;
    }
    
    output[0] = input[0];
    for (size_t i = 1; i < input_len && i < output_len; i++) {
        int16_t delta = (int16_t)input[i] - (int16_t)input[i-1];
        if (delta < -128 || delta > 127) {
            return -1;
        }
        output[i] = (uint8_t)(delta + 128);
    }
    
    return (int)input_len;
}

int compress_data(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return -1;
    }
    
    switch (type) {
        case COMPRESS_NONE:
            if (output_len < input_len) {
                return -1;
            }
            memcpy(output, input, input_len);
            return (int)input_len;
            
        case COMPRESS_RLE:
            return rle_compress(input, input_len, output, output_len);
            
        case COMPRESS_DELTA:
            return delta_compress(input, input_len, output, output_len);
            
        default:
            return -1;
    }
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    
    printf("Enter data to compress (up to 255 bytes, hex digits only): ");
    
    char buffer[512];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *ptr = buffer;
    while (input_len < sizeof(input_data) && isxdigit((unsigned char)*ptr) && isxdigit((unsigned char)*(ptr + 1))) {
        char hex[3] = {*ptr, *(ptr + 1), '\0'};
        unsigned int value;
        if (sscanf(hex, "%2x", &value) != 1) {
            break;
        }
        input_data[input_len++] = (uint8_t)value;
        ptr += 2;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No valid hex data entered\n");
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
    
    enum compression_type comp_type = (enum compression_type)choice;
    
    int result = compress_data(comp_type, input_data, input_len, output_data, sizeof(output_data));
    
    if (result < 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        printf("%02X", input_data[i]);
    }
    printf("\n");
    
    printf("Compressed data (%d bytes): ", result);
    for (int i = 0; i < result; i++) {
        printf("%02X", output_data[i]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result / input_len) * 100.0);
    
    return 0;
}