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

Buffer get_user_input(void) {
    Buffer buffer;
    buffer.data = malloc(MAX_INPUT_SIZE);
    if (buffer.data == NULL) {
        buffer.size = 0;
        return buffer;
    }
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)buffer.data, MAX_INPUT_SIZE, stdin) == NULL) {
        free(buffer.data);
        buffer.size = 0;
        return buffer;
    }
    
    buffer.size = strlen((char*)buffer.data);
    if (buffer.size > 0 && buffer.data[buffer.size - 1] == '\n') {
        buffer.data[buffer.size - 1] = '\0';
        buffer.size--;
    }
    
    return buffer;
}

void print_buffer(Buffer buffer, const char *label) {
    printf("%s: ", label);
    for (size_t i = 0; i < buffer.size; i++) {
        printf("%02X ", buffer.data[i]);
    }
    printf("\n");
}

void cleanup_buffer(Buffer buffer) {
    if (buffer.data != NULL) {
        free(buffer.data);
    }
}

int main(void) {
    CompressFunc compress = compress_rle;
    DecompressFunc decompress = decompress_rle;
    
    Buffer original = get_user_input();
    if (original.data == NULL || original.size == 0) {
        printf("Error: Failed to read input\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", original.size);
    
    Buffer compressed = compress(original);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Error: Compression failed\n");
        cleanup_buffer(original);
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / original.size * 100.0f);
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Error: Decompression failed\n");
        cleanup_buffer(original);
        cleanup_buffer(compressed);
        return 1;
    }
    
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("Verification: SUCCESS - Data matches original\n");
    } else {
        printf("Verification: FAILED - Data corrupted\n");
    }
    
    print_buffer(original, "Original data");
    print_buffer(compressed, "Compressed data");
    print_buffer(decompressed, "Decompressed data");
    
    cleanup_buffer(original);
    cleanup_buffer(compressed);
    cleanup_buffer(decompressed);
    
    return 0;
}