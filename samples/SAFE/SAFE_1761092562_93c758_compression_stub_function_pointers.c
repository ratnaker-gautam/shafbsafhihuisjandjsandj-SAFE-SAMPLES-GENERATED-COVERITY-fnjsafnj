//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(Buffer input);

Buffer compress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size > MAX_INPUT_SIZE) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input.size && 
               input.data[in_pos + count] == current && 
               count < UINT8_MAX) {
            count++;
        }
        
        if (count > 1) {
            output.data[out_pos++] = current;
            output.data[out_pos++] = (uint8_t)count;
        } else {
            output.data[out_pos++] = current;
            output.data[out_pos++] = 1;
        }
        
        in_pos += count;
    }
    
    output.size = out_pos;
    return output;
}

Buffer decompress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size % 2 != 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE) {
        uint8_t value = input.data[in_pos++];
        uint8_t count = input.data[in_pos++];
        
        if (out_pos + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

void print_buffer(Buffer buf) {
    if (buf.data == NULL || buf.size == 0) {
        return;
    }
    
    for (size_t i = 0; i < buf.size; i++) {
        printf("%02X ", buf.data[i]);
    }
    printf("\n");
}

int main() {
    CompressionFunc compress = compress_rle;
    CompressionFunc decompress = decompress_rle;
    
    char input_str[MAX_INPUT_SIZE];
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        return 1;
    }
    
    Buffer input;
    input.data = malloc(MAX_INPUT_SIZE);
    if (input.data == NULL) {
        return 1;
    }
    
    input.size = 0;
    char *token = strtok(input_str, " \n");
    
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) == 1 && byte <= UINT8_MAX) {
            input.data[input.size++] = (uint8_t)byte;
        }
        token = strtok(NULL, " \n");
    }
    
    if (input.size == 0) {
        free(input.data);
        return 1;
    }
    
    printf("Original: ");
    print_buffer(input);
    
    Buffer compressed = compress(input);
    if (compressed.data != NULL && compressed.size > 0) {
        printf("Compressed: ");
        print_buffer(compressed);
        
        Buffer decompressed = decompress(compressed);
        if (decompressed.data != NULL && decompressed.size > 0) {
            printf("Decompressed: ");
            print_buffer(decompressed);
            
            if (input.size == decompressed.size && 
                memcmp(input.data, decompressed.data, input.size) == 0) {
                printf("Verification: SUCCESS\n");
            } else {
                printf("Verification: FAILED\n");
            }
            
            free(decompressed.data);
        }
        
        free(compressed.data);
    }
    
    free(input.data);
    return 0;
}