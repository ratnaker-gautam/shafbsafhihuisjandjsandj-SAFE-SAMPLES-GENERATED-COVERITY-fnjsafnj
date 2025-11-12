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
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }
    }
    
    return 0;
}

int read_input(Buffer* buffer) {
    if (buffer == NULL) {
        return -1;
    }
    
    buffer->size = 0;
    int c;
    
    printf("Enter data to compress (max %d bytes, Ctrl+D to end):\n", MAX_INPUT_SIZE);
    
    while ((c = getchar()) != EOF && buffer->size < MAX_INPUT_SIZE) {
        buffer->data[buffer->size++] = (uint8_t)c;
    }
    
    if (buffer->size == 0) {
        printf("No input received.\n");
        return -1;
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
    
    if (buffer->size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_data;
    Buffer compressed;
    Buffer decompressed;
    
    if (read_input(&input_data) != 0) {
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_data.size);
    print_buffer_hex(&input_data);
    
    if (compress_rle(input_data.data, input_data.size, &compressed) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)compressed.size / input_data.size * 100.0f);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (decompressed.size != input_data.size || 
        memcmp(decompressed.data, input_data.data, input_data.size) != 0) {
        printf("Decompression verification failed.\n");
        return 1;
    }
    
    printf("Decompression verified successfully.\n");
    
    return 0;
}