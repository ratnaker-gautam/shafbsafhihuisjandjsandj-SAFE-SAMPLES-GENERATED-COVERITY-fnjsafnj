//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int compress_rle(const uint8_t* input, size_t input_size, CompressedData* output) {
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
        uint8_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size++] = count;
        output->data[output->size++] = current;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const CompressedData* compressed, uint8_t* output, size_t* output_size) {
    if (compressed == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    size_t decompressed_size = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        if (i + 1 >= compressed->size) {
            return -1;
        }
        
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (decompressed_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_size++] = value;
        }
    }
    
    *output_size = decompressed_size;
    return 0;
}

int read_input(uint8_t* buffer, size_t* size) {
    if (buffer == NULL || size == NULL) {
        return -1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_buffer[MAX_INPUT_SIZE];
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(temp_buffer);
    if (len > 0 && temp_buffer[len - 1] == '\n') {
        temp_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_SIZE) {
        return -1;
    }
    
    memcpy(buffer, temp_buffer, len);
    *size = len;
    return 0;
}

void print_compressed(const CompressedData* compressed) {
    if (compressed == NULL) {
        return;
    }
    
    printf("Compressed data (%zu bytes): ", compressed->size);
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02X ", compressed->data[i]);
    }
    printf("\n");
}

void print_decompressed(const uint8_t* data, size_t size) {
    if (data == NULL) {
        return;
    }
    
    printf("Decompressed data (%zu bytes): ", size);
    for (size_t i = 0; i < size; i++) {
        printf("%c", isprint(data[i]) ? data[i] : '.');
    }
    printf("\n");
}

int main() {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE];
    CompressedData compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    if (read_input(input_buffer, &input_size) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < input_size; i++) {
        printf("%c", input_buffer[i]);
    }
    printf(" (%zu bytes)\n", input_size);
    
    if (compress_rle(input_buffer, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    print_compressed(&compressed);
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_size * 100.0f);
    
    if (decompress_rle(&compressed, output_buffer, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    print_decompressed(output_buffer, decompressed_size);
    
    if (decompressed_size != input_size || 
        memcmp(input_buffer, output_buffer, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Verification successful\n");
    return 0;
}