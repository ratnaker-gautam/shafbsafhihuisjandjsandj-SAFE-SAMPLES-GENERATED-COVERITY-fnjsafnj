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

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t copy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t copy_size = input_size < output_size ? input_size : output_size;
    memcpy(output, input, copy_size);
    return copy_size;
}

size_t copy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return copy_compress(input, input_size, output, output_size);
}

Buffer read_input(void) {
    Buffer buf = {NULL, 0};
    char input[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return buf;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return buf;
    }
    
    buf.data = malloc(len);
    if (buf.data == NULL) {
        return buf;
    }
    
    memcpy(buf.data, input, len);
    buf.size = len;
    return buf;
}

void process_compression(CompressFunc compress, DecompressFunc decompress, const char *name) {
    Buffer input = read_input();
    if (input.data == NULL || input.size == 0) {
        printf("No input data received.\n");
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress(input.data, input.size, compressed, sizeof(compressed));
    
    printf("%s compression: Input %zu bytes, Output %zu bytes\n", name, input.size, compressed_size);
    
    if (compressed_size > 0) {
        uint8_t decompressed[MAX_INPUT_SIZE];
        size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
        
        if (decompressed_size == input.size && memcmp(input.data, decompressed, input.size) == 0) {
            printf("Decompression successful: %zu bytes\n", decompressed_size);
        } else {
            printf("Decompression failed or data mismatch\n");
        }
    }
    
    free(input.data);
}

int main(void) {
    CompressFunc compressors[] = {run_length_encode, copy_compress};
    DecompressFunc decompressors[] = {run_length_decode, copy_decompress};
    const char *names[] = {"Run-length", "Copy"};
    const size_t num_methods = sizeof(compressors) / sizeof(compressors[0]);
    
    printf("Compression Method Demo\n");
    printf("=======================\n");
    
    for (size_t i = 0; i < num_methods; i++) {
        printf("\nMethod %zu: %s\n", i + 1, names[i]);
        process_compression(compressors[i], decompressors[i], names[i]);
    }
    
    return 0;
}