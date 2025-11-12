//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(Buffer);
typedef Buffer (*DecompressionFunc)(Buffer);

Buffer run_length_encode(Buffer input) {
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
    size_t i = 0;
    
    while (i < input.size && out_pos < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[i];
        uint8_t count = 1;
        
        while (i + count < input.size && count < 255 && 
               input.data[i + count] == current) {
            count++;
        }
        
        if (out_pos + 2 >= MAX_OUTPUT_SIZE) break;
        
        output.data[out_pos++] = count;
        output.data[out_pos++] = current;
        i += count;
    }
    
    output.size = out_pos;
    return output;
}

Buffer run_length_decode(Buffer input) {
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
    size_t i = 0;
    
    while (i < input.size && out_pos < MAX_OUTPUT_SIZE) {
        uint8_t count = input.data[i++];
        uint8_t value = input.data[i++];
        
        if (out_pos + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

void print_buffer_hex(Buffer buf) {
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

void process_compression(CompressionFunc compress, DecompressionFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
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
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed);
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        if (isprint(decompressed.data[i])) {
            printf("%c", decompressed.data[i]);
        } else {
            printf("?");
        }
    }
    printf("\n");
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Round-trip successful!\n");
    } else {
        printf("Round-trip failed!\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressionFunc compress_func = run_length_encode;
    DecompressionFunc decompress_func = run_length_decode;
    
    printf("Simple Run-Length Encoding Demo\n");
    printf("===============================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}