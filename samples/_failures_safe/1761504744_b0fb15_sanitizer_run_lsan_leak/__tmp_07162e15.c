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
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[in_pos];
        uint8_t count = 1;
        
        while (in_pos + count < input.size && 
               input.data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_pos + 2 >= MAX_OUTPUT_SIZE) break;
        
        output.data[out_pos++] = count;
        output.data[out_pos++] = current;
        in_pos += count;
    }
    
    output.size = out_pos;
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
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE) {
        uint8_t count = input.data[in_pos++];
        uint8_t value = input.data[in_pos++];
        
        if (out_pos + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

Buffer get_user_input() {
    Buffer input;
    input.data = malloc(MAX_INPUT_SIZE);
    if (input.data == NULL) {
        input.size = 0;
        return input;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input.data, MAX_INPUT_SIZE, stdin) == NULL) {
        free(input.data);
        input.size = 0;
        return input;
    }
    
    input.size = strlen((char*)input.data);
    if (input.size > 0 && input.data[input.size - 1] == '\n') {
        input.data[input.size - 1] = '\0';
        input.size--;
    }
    
    return input;
}

void print_buffer(Buffer buf, const char* label) {
    printf("%s (%zu bytes): ", label, buf.size);
    for (size_t i = 0; i < buf.size; i++) {
        if (buf.data[i] >= 32 && buf.data[i] <= 126) {
            printf("%c", buf.data[i]);
        } else {
            printf("[0x%02X]", buf.data[i]);
        }
    }
    printf("\n");
}

int main() {
    CompressFunc compress = compress_rle;
    DecompressFunc decompress = decompress_rle;
    
    Buffer original = get_user_input();
    if (original.data == NULL || original.size == 0) {
        printf("Error: Failed to read input\n");
        return 1;
    }
    
    printf("\nOriginal data:\n");
    print_buffer(original, "Input");
    
    Buffer compressed = compress(original);
    if (compressed.data == NULL) {
        printf("Error: Compression failed\n");
        free(original.data);
        return 1;
    }
    
    printf("\nAfter compression:\n");
    print_buffer(compressed, "Compressed");
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / original.size * 100);
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL) {
        printf("Error: Decompression failed\n");
        free(original.data);
        free(compressed.data);
        return 1;
    }
    
    printf("\nAfter decompression:\n");
    print_buffer(decompressed, "Decompressed");
    
    int match = (original.size == decompressed.size) && 
                (memcmp(original.data, decompressed.data, original.size) == 0);
    printf("Data integrity: %s\n", match ? "PASS" : "FAIL");
    
    free(original.data);
    free(compressed.data);
    free(decompressed.data);
    
    return match ? 0 : 1;
}