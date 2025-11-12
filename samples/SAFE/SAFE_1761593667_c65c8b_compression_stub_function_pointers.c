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

typedef Buffer (*CompressFunc)(Buffer);
typedef Buffer (*DecompressFunc)(Buffer);

Buffer compress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0) {
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

void print_buffer(Buffer buf) {
    if (buf.data == NULL || buf.size == 0) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t i = 0; i < buf.size; i++) {
        printf("%02X ", buf.data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf.size % 16 != 0) printf("\n");
}

int main() {
    CompressFunc compress = compress_rle;
    DecompressFunc decompress = decompress_rle;
    
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    Buffer original;
    original.data = test_data;
    original.size = sizeof(test_data);
    
    printf("Original data:\n");
    print_buffer(original);
    
    Buffer compressed = compress(original);
    printf("Compressed data:\n");
    print_buffer(compressed);
    
    Buffer decompressed = decompress(compressed);
    printf("Decompressed data:\n");
    print_buffer(decompressed);
    
    int match = 1;
    if (original.size != decompressed.size) {
        match = 0;
    } else {
        for (size_t i = 0; i < original.size; i++) {
            if (original.data[i] != decompressed.data[i]) {
                match = 0;
                break;
            }
        }
    }
    
    printf("Round-trip successful: %s\n", match ? "Yes" : "No");
    
    if (compressed.data != NULL) free(compressed.data);
    if (decompressed.data != NULL) free(decompressed.data);
    
    return 0;
}