//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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

typedef Buffer (*CompressFunc)(const Buffer*);
typedef Buffer (*DecompressFunc)(const Buffer*);

Buffer dummy_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) {
        return output;
    }
    
    output.size = input->size * 2;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i * 2] = input->data[i];
        output.data[i * 2 + 1] = input->data[i] ^ 0xFF;
    }
    
    return output;
}

Buffer dummy_decompress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0 || input->size % 2 != 0) {
        return output;
    }
    
    output.size = input->size / 2;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    for (size_t i = 0; i < output.size; i++) {
        if (input->data[i * 2 + 1] != (input->data[i * 2] ^ 0xFF)) {
            free(output.data);
            output.data = NULL;
            output.size = 0;
            return output;
        }
        output.data[i] = input->data[i * 2];
    }
    
    return output;
}

void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (buf->size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    CompressFunc compress = dummy_compress;
    DecompressFunc decompress = dummy_decompress;
    
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer input_buf = {(uint8_t*)input_str, input_len};
    
    printf("\nOriginal data (%zu bytes):\n", input_buf.size);
    print_buffer_hex(&input_buf);
    
    Buffer compressed = compress(&input_buf);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (decompressed.size == input_buf.size && 
        memcmp(decompressed.data, input_buf.data, input_buf.size) == 0) {
        printf("\nRound-trip successful!\n");
    } else {
        printf("\nRound-trip failed!\n");
    }
    
    printf("\nDecompressed text: ");
    fwrite(decompressed.data, 1, decompressed.size, stdout);
    printf("\n");
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}