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
    if (input.data == NULL) {
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

void print_buffer_hex(Buffer buf) {
    if (buf.data == NULL || buf.size == 0) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t i = 0; i < buf.size; i++) {
        printf("%02X ", buf.data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (buf.size % 16 != 0) {
        printf("\n");
    }
}

void print_buffer_ascii(Buffer buf) {
    if (buf.data == NULL || buf.size == 0) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t i = 0; i < buf.size; i++) {
        if (isprint(buf.data[i])) {
            printf("%c", buf.data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

void free_buffer(Buffer buf) {
    if (buf.data != NULL) {
        free(buf.data);
    }
}

int main() {
    CompressionFunc compress_funcs[] = {run_length_encode, null_compress};
    DecompressionFunc decompress_funcs[] = {run_length_decode, null_decompress};
    const char *method_names[] = {"Run-length encoding", "Null compression"};
    const int num_methods = 2;
    
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
        printf("Empty input\n");
        return 1;
    }
    
    Buffer original = {(uint8_t*)input_str, input_len};
    
    printf("\nOriginal data (%zu bytes):\n", original.size);
    print_buffer_hex(original);
    printf("ASCII: ");
    print_buffer_ascii(original);
    
    for (int i = 0; i < num_methods; i++) {
        printf("\n--- %s ---\n", method_names[i]);
        
        Buffer compressed = compress_funcs[i](original);
        if (compressed.data == NULL) {
            printf("Compression failed\n");
            continue;
        }
        
        printf("Compressed (%zu bytes):\n", compressed.size);