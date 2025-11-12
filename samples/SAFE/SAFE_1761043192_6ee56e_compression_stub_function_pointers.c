//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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

typedef Buffer (*CompressFunc)(Buffer);
typedef Buffer (*DecompressFunc)(Buffer);

Buffer null_compress(Buffer input) {
    Buffer output;
    if (input.size == 0 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    output.size = input.size + 1;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    memcpy(output.data, input.data, input.size);
    output.data[input.size] = 0xFF;
    return output;
}

Buffer null_decompress(Buffer input) {
    Buffer output;
    if (input.size == 0 || input.data == NULL || input.size < 2) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    if (input.data[input.size - 1] != 0xFF) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    output.size = input.size - 1;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    memcpy(output.data, input.data, output.size);
    return output;
}

int validate_buffer(Buffer buf) {
    return buf.data != NULL && buf.size > 0 && buf.size <= MAX_INPUT_SIZE;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
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
    if (!validate_buffer(compressed)) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = decompress(compressed);
    if (!validate_buffer(decompressed)) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Decompression successful: ");
        fwrite(decompressed.data, 1, decompressed.size, stdout);
        printf("\n");
    } else {
        printf("Decompression verification failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compress_funcs[] = {null_compress};
    DecompressFunc decompress_funcs[] = {null_decompress};
    
    process_compression(compress_funcs[0], decompress_funcs[0]);
    
    return 0;
}