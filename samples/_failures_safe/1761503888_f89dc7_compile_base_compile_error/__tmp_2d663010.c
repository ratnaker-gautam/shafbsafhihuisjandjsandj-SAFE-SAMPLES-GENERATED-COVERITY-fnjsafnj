//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(Buffer);

Buffer compress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size > MAX_INPUT_SIZE) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t max_output = input.size * COMPRESSED_MULTIPLIER;
    if (max_output / COMPRESSED_MULTIPLIER != input.size) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t *output = malloc(max_output);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input.size) {
        if (out_index + 2 >= max_output) {
            free(output);
            Buffer empty = {NULL, 0};
            return empty;
        }
        
        uint8_t current = input.data[i];
        uint8_t count = 1;
        
        while (i + count < input.size && count < UINT8_MAX && 
               input.data[i + count] == current) {
            count++;
        }
        
        output[out_index++] = count;
        output[out_index++] = current;
        i += count;
    }
    
    Buffer result = {output, out_index};
    return result;
}

Buffer compress_copy(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size > MAX_INPUT_SIZE) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t *output = malloc(input.size);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    memcpy(output, input.data, input.size);
    Buffer result = {output, input.size};
    return result;
}

Buffer decompress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size % 2 != 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t output_size = 0;
    for (size_t i = 0; i < input.size; i += 2) {
        if (input.data[i] == 0) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        if (output_size > SIZE_MAX - input.data[i]) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        output_size += input.data[i];
    }
    
    if (output_size > MAX_INPUT_SIZE) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t *output = malloc(output_size);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    for (size_t i = 0; i < input.size; i += 2) {
        uint8_t count = input.data[i];
        uint8_t value = input.data[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (out_index >= output_size) {
                free(output);
                Buffer empty = {NULL, 0};
                return empty;
            }
            output[out_index++] = value;
        }
    }
    
    Buffer result = {output, output_size};
    return result;
}

Buffer decompress_copy(Buffer input) {
    return compress_copy(input);
}

void process_compression(CompressionFunc compress, CompressionFunc decompress, const char* name) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress with %s (max %d characters): ", name, MAX_INPUT_SIZE);
    
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
    
    Buffer input = {(uint8_t*)input_str, len};
    Buffer compressed = compress(input);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", len, compressed.size);
    
    if (compressed.size < len) {
        printf("Compression ratio: %.2f\n", (float)compressed.size / len);
    } else {
        printf("No compression achieved\n");
    }
    
    Buffer decompressed = decompress(compressed);
    
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == len && memcmp(decompressed.data, input_str, len) == 0) {
        printf("Decompression verified: