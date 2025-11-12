//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct compression_params {
    enum compression_type type;
    uint32_t max_size;
    uint8_t quality;
};

int validate_params(const struct compression_params *params) {
    if (params == NULL) return 0;
    if (params->type > COMP_HUFFMAN) return 0;
    if (params->max_size == 0 || params->max_size > 1048576) return 0;
    if (params->quality > 100) return 0;
    return 1;
}

size_t compress_data(const uint8_t *input, size_t input_len, 
                    uint8_t *output, size_t output_len,
                    const struct compression_params *params) {
    if (input == NULL || output == NULL || params == NULL) return 0;
    if (input_len == 0 || output_len == 0) return 0;
    if (!validate_params(params)) return 0;
    
    size_t result_len = 0;
    
    switch (params->type) {
        case COMP_NONE:
            if (output_len < input_len) return 0;
            memcpy(output, input, input_len);
            result_len = input_len;
            break;
            
        case COMP_RLE:
            if (input_len > output_len) return 0;
            for (size_t i = 0; i < input_len && result_len < output_len - 1; i++) {
                uint8_t count = 1;
                while (i + 1 < input_len && input[i] == input[i + 1] && count < 255) {
                    count++;
                    i++;
                }
                if (result_len + 2 > output_len) return 0;
                output[result_len++] = count;
                output[result_len++] = input[i];
            }
            break;
            
        case COMP_LZW:
            if (output_len < input_len + 4) return 0;
            output[result_len++] = 'L';
            output[result_len++] = 'Z';
            output[result_len++] = 'W';
            output[result_len++] = (uint8_t)(params->quality & 0xFF);
            if (output_len - result_len < input_len) return 0;
            memcpy(output + result_len, input, input_len);
            result_len += input_len;
            break;
            
        case COMP_HUFFMAN:
            if (output_len < input_len + 8) return 0;
            output[result_len++] = 'H';
            output[result_len++] = 'U';
            output[result_len++] = 'F';
            output[result_len++] = (uint8_t)(params->quality & 0xFF);
            uint32_t crc = 0;
            for (size_t i = 0; i < input_len; i++) {
                crc = (crc << 1) ^ input[i];
            }
            output[result_len++] = (crc >> 24) & 0xFF;
            output[result_len++] = (crc >> 16) & 0xFF;
            output[result_len++] = (crc >> 8) & 0xFF;
            output[result_len++] = crc & 0xFF;
            if (output_len - result_len < input_len) return 0;
            memcpy(output + result_len, input, input_len);
            result_len += input_len;
            break;
            
        default:
            return 0;
    }
    
    return result_len;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    struct compression_params params;
    
    printf("Enter compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    int type_input;
    if (scanf("%d", &type_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    params.type = (enum compression_type)type_input;
    
    printf("Enter maximum size (bytes, 1-1048576): ");
    if (scanf("%u", &params.max_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter quality (0-100): ");
    if (scanf("%hhu", &params.quality) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_params(&params)) {
        printf("Invalid parameters\n");
        return 1;
    }
    
    printf("Enter input data (up to 255 characters): ");
    getchar();
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Failed to read input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[--input_len] = '\0';
    }
    
    if (input_len > sizeof(input_data)) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input_data, buffer, input_len);
    
    size_t compressed_size = compress_data(input_data, input_len, output_data, 
                                         sizeof(output_data), &params);
    
    if (compressed_size == 0) {