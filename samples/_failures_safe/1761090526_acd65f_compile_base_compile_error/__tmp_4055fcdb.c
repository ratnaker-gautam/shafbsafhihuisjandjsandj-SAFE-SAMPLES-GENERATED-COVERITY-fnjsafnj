//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        in_idx += count;
    }
    
    output->size = out_idx;
    return 0;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0 || input->size % 2 != 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if (i < buf->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int read_input_buffer(Buffer* buf) {
    if (buf == NULL) return -1;
    
    printf("Enter data as hex bytes (e.g., 41 42 43): ");
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    
    char* token = strtok(line, " ");
    size_t count = 0;
    
    while (token != NULL && count < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            return -1;
        }
        
        int valid = 1;
        for (int i = 0; i < 2; i++) {
            if (!isxdigit(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            return -1;
        }
        
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) {
            return -1;
        }
        
        if (byte > 255) {
            return -1;
        }
        
        buf->data[count++] = (uint8_t)byte;
        token = strtok(NULL, " ");
    }
    
    buf->size = count;
    return 0;
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("RLE Compression/Decompression Tool\n");
    
    if (read_input_buffer(&input) != 0) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (input.size == 0) {
        printf("Error: No input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input.size);
    print_buffer_hex(&input);
    
    if (compress_rle(&input, &compressed) != 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(&compressed);
    
    if (decompress_rle(&compressed, &decompressed) != 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (input.size != decompressed.size) {
        printf("Error: Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Error: Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Success: Compression and decompression verified\n");
    double ratio = (compressed.size * 100