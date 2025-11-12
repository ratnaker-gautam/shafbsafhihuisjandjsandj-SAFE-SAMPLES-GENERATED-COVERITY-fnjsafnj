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

typedef Buffer (*CompressFunc)(Buffer);
typedef Buffer (*DecompressFunc)(Buffer);

Buffer dummy_compress(Buffer input) {
    Buffer output;
    if (input.size == 0 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size + 4;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    output.data[0] = 'C';
    output.data[1] = 'M';
    output.data[2] = 'P';
    output.data[3] = 'R';
    memcpy(output.data + 4, input.data, input.size);
    
    return output;
}

Buffer dummy_decompress(Buffer input) {
    Buffer output;
    if (input.size < 4 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    if (input.data[0] != 'C' || input.data[1] != 'M' || 
        input.data[2] != 'P' || input.data[3] != 'R') {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size - 4;
    if (output.size == 0) {
        output.data = NULL;
        return output;
    }
    
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input.data + 4, output.size);
    return output;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    int choice;
    
    printf("Enter 1 to compress, 2 to decompress: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    while (getchar() != '\n');
    
    printf("Enter data: ");
    if (fgets(input_str, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Failed to read input\n");
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
    
    Buffer input, output;
    input.data = (uint8_t*)input_str;
    input.size = len;
    
    if (choice == 1) {
        output = compress(input);
        if (output.data == NULL) {
            printf("Compression failed\n");
            return;
        }
        printf("Compressed data: ");
        for (size_t i = 0; i < output.size; i++) {
            printf("%02X", output.data[i]);
        }
        printf("\n");
        free(output.data);
    } else if (choice == 2) {
        if (len % 2 != 0) {
            printf("Invalid compressed data format\n");
            return;
        }
        
        size_t hex_len = len / 2;
        if (hex_len > MAX_OUTPUT_SIZE) {
            printf("Input too large\n");
            return;
        }
        
        uint8_t *hex_data = malloc(hex_len);
        if (hex_data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        
        for (size_t i = 0; i < hex_len; i++) {
            unsigned int byte;
            if (sscanf(input_str + i * 2, "%02x", &byte) != 1) {
                printf("Invalid hex data\n");
                free(hex_data);
                return;
            }
            hex_data[i] = (uint8_t)byte;
        }
        
        input.data = hex_data;
        input.size = hex_len;
        
        output = decompress(input);
        free(hex_data);
        
        if (output.data == NULL) {
            printf("Decompression failed\n");
            return;
        }
        
        printf("Decompressed data: ");
        fwrite(output.data, 1, output.size, stdout);
        printf("\n");
        free(output.data);
    } else {
        printf("Invalid choice\n");
    }
}

int main() {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}