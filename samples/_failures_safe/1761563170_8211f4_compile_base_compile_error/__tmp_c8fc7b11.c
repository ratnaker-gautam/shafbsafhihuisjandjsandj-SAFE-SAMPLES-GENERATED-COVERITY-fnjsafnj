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
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
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
        
        output[out_pos++] = count;
        output[out_pos++] = current;
        i += count;
    }
    
    Buffer result = {output, out_pos};
    return result;
}

Buffer run_length_decode(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size % 2 != 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_pos = 0;
    
    for (size_t i = 0; i < input.size && out_pos < MAX_OUTPUT_SIZE; i += 2) {
        uint8_t count = input.data[i];
        uint8_t value = input.data[i + 1];
        
        for (uint8_t j = 0; j < count && out_pos < MAX_OUTPUT_SIZE; j++) {
            output[out_pos++] = value;
        }
    }
    
    Buffer result = {output, out_pos};
    return result;
}

Buffer null_compress(Buffer input) {
    if (input.data == NULL || input.size == 0) {
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
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / len * 100.0f);
    
    Buffer decompressed = decompress(compressed);
    
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == len && 
        memcmp(decompressed.data, input_str, len) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressionFunc compressors[] = {run_length_encode, null_compress};
    DecompressionFunc decompressors[] = {run_length_decode, null_decompress};
    const char *names[] = {"Run-Length Encoding", "Null Compression"};
    int num_methods = sizeof(compressors) / sizeof(compressors[0]);
    
    printf("Available compression methods:\n");
    for (int i = 0; i < num_methods; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (1-%d): ", num_methods);
    
    char choice_str[10];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Error reading choice\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    if (choice < 1 || choice > num_methods) {
        printf("Invalid choice\n");
        return 1;