//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
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
    for (size_t i = 1; i < input_len && i < output_size; i++) {
        int16_t delta = (int16_t)input[i] - (int16_t)input[i-1];
        if (delta < -128 || delta > 127) {
            return -1;
        }
        output[i] = (uint8_t)(delta + 128);
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
    char buffer[256];
    enum compression_type comp_type = COMPRESS_NONE;
    
    printf("Enter up to 256 bytes of data (hex values, space separated): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    char *token = strtok(buffer, " \n");
    while (token != NULL && input_len < 256) {
        if (strlen(token) != 2) {
            printf("Invalid hex byte: %s\n", token);
            return 1;
        }
        
        int valid = 1;
        for (int i = 0; i < 2; i++) {
            if (!isxdigit((unsigned char)token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            printf("Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned long value = strtoul(token, NULL, 16);
        if (value > 255) {
            printf("Byte value out of range: %s\n", token);
            return 1;
        }
        
        input_data[input_len++] = (uint8_t)value;
        token = strtok(NULL, " \n");
    }
    
    if (input_len == 0) {
        printf("No input data provided\n");
        return 1;
    }
    
    printf("Select compression type (0=None, 1=RLE, 2=Delta): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    char *endptr;
    long type_val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || (*endptr != '\n' && *endptr != '\0') || type_val < 0 || type_val >= COMPRESS_TYPE_COUNT) {
        printf("Invalid compression type\n");
        return 1;
    }
    comp_type = (enum compression_type)type_val;
    
    int result = -1;
    switch (comp_type) {
        case COMPRESS_NONE:
            result = compress_none(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_RLE:
            result = compress_rle(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_DELTA:
            result = compress_delta(input_data, input_len, output_data, sizeof(output_data));
            break;
        default:
            printf("Unknown compression type\n");
            return 1;
    }
    
    if (result < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%d bytes): ", result);
    for (int i = 0; i < result && i < 512; i++) {
        printf("%02X ", output_data[i]);