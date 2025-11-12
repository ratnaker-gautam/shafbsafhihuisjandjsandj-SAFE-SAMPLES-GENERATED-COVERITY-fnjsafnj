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
typedef Buffer (*DecompressionFunc)(Buffer input);

Buffer run_length_encode(Buffer input) {
    if (input.data == NULL || input.size == 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input.size && out_index < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[i];
        uint8_t count = 1;
        
        while (i + count < input.size && count < 255 && 
               input.data[i + count] == current) {
            count++;
        }
        
        output[out_index++] = count;
        output[out_index++] = current;
        i += count;
    }
    
    Buffer result;
    result.data = output;
    result.size = out_index;
    return result;
}

Buffer run_length_decode(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size % 2 != 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t total_size = 0;
    for (size_t i = 0; i < input.size; i += 2) {
        if (input.data[i] == 0) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        total_size += input.data[i];
        if (total_size > MAX_OUTPUT_SIZE) {
            Buffer empty = {NULL, 0};
            return empty;
        }
    }
    
    uint8_t *output = malloc(total_size);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    for (size_t i = 0; i < input.size; i += 2) {
        uint8_t count = input.data[i];
        uint8_t value = input.data[i + 1];
        
        for (uint8_t j = 0; j < count && out_index < total_size; j++) {
            output[out_index++] = value;
        }
    }
    
    Buffer result;
    result.data = output;
    result.size = total_size;
    return result;
}

void process_compression(CompressionFunc compress, DecompressionFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    Buffer input;
    input.data = (uint8_t*)input_str;
    input.size = len;
    
    Buffer compressed = compress(input);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed text (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressionFunc compress_func = run_length_encode;
    DecompressionFunc decompress_func = run_length_decode;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}