//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
               count < 255) {
            count++;
        }
        
        if (out_pos + 2 >= MAX_OUTPUT_SIZE) break;
        
        output.data[out_pos++] = (uint8_t)count;
        output.data[out_pos++] = current;
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
        uint8_t count = input.data[in_pos++];
        uint8_t value = input.data[in_pos++];
        
        if (out_pos + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

Buffer process_data(Buffer input, CompressionFunc func) {
    if (func == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    return func(input);
}

void print_buffer(Buffer buf, const char *label) {
    if (label) printf("%s: ", label);
    for (size_t i = 0; i < buf.size; i++) {
        printf("%02X ", buf.data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    Buffer input;
    input.data = test_data;
    input.size = sizeof(test_data);
    
    printf("Original data: ");
    for (size_t i = 0; i < input.size; i++) {
        printf("%c", input.data[i]);
    }
    printf("\n");
    
    CompressionFunc compress = compress_rle;
    CompressionFunc decompress = decompress_rle;
    
    Buffer compressed = process_data(input, compress);
    if (compressed.data != NULL && compressed.size > 0) {
        print_buffer(compressed, "Compressed");
        
        Buffer decompressed = process_data(compressed, decompress);
        if (decompressed.data != NULL && decompressed.size > 0) {
            print_buffer(decompressed, "Decompressed");
            
            printf("Verification: ");
            for (size_t i = 0; i < decompressed.size; i++) {
                printf("%c", decompressed.data[i]);
            }
            printf("\n");
            
            free(decompressed.data);
        }
        free(compressed.data);
    }
    
    return 0;
}