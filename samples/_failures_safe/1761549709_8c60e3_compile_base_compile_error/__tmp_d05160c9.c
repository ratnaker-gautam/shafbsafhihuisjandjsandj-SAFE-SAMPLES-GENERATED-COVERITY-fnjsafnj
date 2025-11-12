//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            if (output->size + 3 > MAX_OUTPUT_SIZE) {
                return -1;
            }
            output->data[output->size++] = 0xFF;
            output->data[output->size++] = current;
            output->data[output->size++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && output->size < MAX_OUTPUT_SIZE; j++) {
                output->data[output->size++] = current;
            }
        }
        
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count == 0) {
                return -1;
            }
            
            if (output->size + count > MAX_OUTPUT_SIZE) {
                return -1;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output->data[output->size++] = value;
            }
            
            i += 3;
        } else {
            output->data[output->size++] = input[i];
            i++;
        }
    }
    
    return 0;
}

int read_input(Buffer* buffer) {
    if (buffer == NULL) {
        return -1;
    }
    
    buffer->size = 0;
    printf("Enter data to compress (max %d bytes, end with empty line):\n", MAX_INPUT_SIZE);
    
    char line[256];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        if (buffer->size + len >= MAX_INPUT_SIZE) {
            break;
        }
        
        memcpy(&buffer->data[buffer->size], line, len);
        buffer->size += len;
    }
    
    return 0;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    
    if (buffer->size > 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (read_input(&input_data) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_data.size == 0) {
        printf("No input data provided\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_data.size);
    print_buffer_hex(&input_data);
    
    if (compress_rle(input_data.data, input_data.size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    if (compressed.size > 0) {
        if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
        
        printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
        print_buffer_hex(&decompressed);
        
        if (decompressed.size == input_data.size && 
            memcmp(decompressed.data, input_data.data, input_data