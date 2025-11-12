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
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input.size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[i];
        uint8_t count = 1;
        
        while (i + count < input.size && count < 255 && 
               input.data[i + count] == current) {
            count++;
        }
        
        output.data[out_idx++] = count;
        output.data[out_idx++] = current;
        i += count;
    }
    
    output.size = out_idx;
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
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input.size && out_idx < MAX_OUTPUT_SIZE) {
        uint8_t count = input.data[i++];
        uint8_t value = input.data[i++];
        
        for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
            output.data[out_idx++] = value;
        }
    }
    
    output.size = out_idx;
    return output;
}

Buffer compress_null(Buffer input) {
    if (input.data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(input.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input.data, input.size);
    output.size = input.size;
    return output;
}

Buffer decompress_null(Buffer input) {
    return compress_null(input);
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
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
    printf("Original size: %zu bytes\n", input.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.size / input.size * 100.0f);
    }
    
    Buffer decompressed = decompress(compressed);
    
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compressors[] = {compress_rle, compress_null};
    DecompressFunc decompressors[] = {decompress_rle, decompress_null};
    const char *names[] = {"Run-Length Encoding", "Null (no compression)"};
    
    printf("Available compression methods:\n");
    for (int i = 0; i < 2; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (1-2): ");
    
    char choice_str[10];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Invalid selection\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    if (choice < 1 || choice > 2) {
        printf("Invalid selection\n");
        return 1;
    }
    
    process_compression(compressors[choice - 1], decompressors