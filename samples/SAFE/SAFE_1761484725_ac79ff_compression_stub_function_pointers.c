//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
    }
    
    return out_idx;
}

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    uint8_t temp[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets((char*)temp, sizeof(temp), stdin) == NULL) {
        return buffer;
    }
    
    size_t len = strlen((char*)temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return buffer;
    }
    
    buffer.data = malloc(len);
    if (buffer.data == NULL) {
        return buffer;
    }
    
    memcpy(buffer.data, temp, len);
    buffer.size = len;
    return buffer;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer input = read_input();
    if (input.data == NULL || input.size == 0) {
        printf("No valid input received\n");
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    size_t compressed_size = compress(input.data, input.size, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        free(input.data);
        return;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input.size, compressed_size);
    
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        free(input.data);
        return;
    }
    
    if (decompressed_size != input.size || memcmp(input.data, decompressed, input.size) != 0) {
        printf("Round-trip verification failed\n");
    } else {
        printf("Round-trip verification successful\n");
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size && i < 50; i++) {
        printf("%c", decompressed[i]);
    }
    if (decompressed_size > 50) printf("...");
    printf("\n");
    
    free(input.data);
}

int main(void) {
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Simple Run-Length Encoding Compression Demo\n");
    process_compression(compress_func, decompress_func);
    
    return 0;
}