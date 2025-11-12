//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

void compress_rle_recursive(const uint8_t* input, size_t pos, size_t len, Buffer* output, size_t* out_pos) {
    if (pos >= len) return;
    
    uint8_t current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (*out_pos + 2 > MAX_INPUT_SIZE) return;
    
    output->data[(*out_pos)++] = count;
    output->data[(*out_pos)++] = current;
    
    compress_rle_recursive(input, pos + count, len, output, out_pos);
}

int decompress_rle_recursive(const uint8_t* compressed, size_t pos, size_t len, Buffer* output, size_t* out_pos) {
    if (pos >= len) return 1;
    
    if (pos + 1 >= len) return 0;
    
    uint8_t count = compressed[pos];
    uint8_t value = compressed[pos + 1];
    
    if (*out_pos + count > MAX_INPUT_SIZE) return 0;
    
    for (size_t i = 0; i < count; i++) {
        output->data[(*out_pos)++] = value;
    }
    
    return decompress_rle_recursive(compressed, pos + 2, len, output, out_pos);
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input string (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input.data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input.size = strlen((char*)input.data);
    if (input.size > 0 && input.data[input.size - 1] == '\n') {
        input.data[--input.size] = '\0';
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    size_t comp_pos = 0;
    compress_rle_recursive(input.data, 0, input.size, &compressed, &comp_pos);
    compressed.size = comp_pos;
    
    printf("Original size: %zu\n", input.size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f\n", (float)compressed.size / input.size);
    
    printf("Original: ");
    print_buffer_hex(&input);
    
    printf("Compressed: ");
    print_buffer_hex(&compressed);
    
    size_t decomp_pos = 0;
    int decomp_result = decompress_rle_recursive(compressed.data, 0, compressed.size, &decompressed, &decomp_pos);
    decompressed.size = decomp_pos;
    
    if (!decomp_result) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    print_buffer_hex(&decompressed);
    
    if (input.size != decompressed.size || memcmp(input.data, decompressed.data, input.size) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Verification successful\n");
    return 0;
}