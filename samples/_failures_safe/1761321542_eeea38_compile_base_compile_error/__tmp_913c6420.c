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
        
        while (i + count < input.size && input.data[i + count] == current && count < 255) {
            count++;
        }
        
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
        
        for (uint8_t j = 0; j < count && out_pos < MAX_OUTPUT_SIZE; j++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

Buffer null_compress(Buffer input) {
    if (input.data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.size = input.size;
    output.data = malloc(input.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input.data, input.size);
    return output;
}

Buffer null_decompress(Buffer input) {
    return null_compress(input);
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
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size && i < 50; i++) {
        if (isprint(compressed.data[i])) {
            printf("%c", compressed.data[i]);
        } else {
            printf("[0x%02X]", compressed.data[i]);
        }
    }
    printf("\n");
    
    Buffer decompressed = decompress(compressed);
    
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size && i < 50; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressionFunc compressors[] = {run_length_encode, null_compress};
    DecompressionFunc decompressors[] = {run_length_decode, null_decompress};
    const char* names[] = {"Run-Length Encoding", "Null Compression"};
    int num_methods = sizeof(compressors) / sizeof(compressors[0]);
    
    printf("Available compression methods:\n");
    for (int i = 0; i < num_methods; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (