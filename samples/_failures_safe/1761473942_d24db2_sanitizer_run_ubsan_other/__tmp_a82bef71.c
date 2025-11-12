//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)
#define RUN_LENGTH_MAX 255
#define BYTE_MASK 0xFF

typedef struct {
    unsigned char data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const unsigned char* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void simple_compress(const unsigned char* input, size_t input_size, Buffer* output) {
    output->size = 0;
    if (input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char current = input[i];
        unsigned char count = 1;
        
        while (count < RUN_LENGTH_MAX && i + count < input_size && input[i + count] == current) {
            count++;
        }
        
        if (count > 1 || current >= 0xF0) {
            if (output->size + 2 > MAX_OUTPUT_SIZE) break;
            output->data[output->size++] = 0xFF;
            output->data[output->size++] = count;
            output->data[output->size++] = current;
            i += count;
        } else {
            if (output->size + 1 > MAX_OUTPUT_SIZE) break;
            output->data[output->size++] = current;
            i++;
        }
    }
}

static void simple_decompress(const unsigned char* input, size_t input_size, Buffer* output) {
    output->size = 0;
    if (input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            unsigned char count = input[i + 1];
            unsigned char value = input[i + 2];
            
            if (count == 0 || output->size + count > MAX_INPUT_SIZE) {
                break;
            }
            
            for (unsigned char j = 0; j < count; j++) {
                output->data[output->size++] = value;
            }
            i += 3;
        } else {
            if (output->size + 1 > MAX_INPUT_SIZE) break;
            output->data[output->size++] = input[i];
            i++;
        }
    }
}

static int compare_buffers(const Buffer* a, const Buffer* b) {
    if (a->size != b->size) return 0;
    return memcmp(a->data, b->data, a->size) == 0;
}

static void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < buf->size) printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes as hex values (space separated): ", input_size);
    for (int i = 0; i < input_size; i++) {
        unsigned int byte_val;
        if (scanf("%02x", &byte_val) != 1) {
            fprintf(stderr, "Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte_val > BYTE_MASK) {
            fprintf(stderr, "Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (unsigned char)byte_val;
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Input validation failed\n");
        return EXIT_FAILURE;
    }
    
    simple_compress(input_data, input_size, &compressed);
    simple_decompress(compressed.data, compressed.size, &decompressed);
    
    if (!compare_buffers((Buffer*)&input_data, &decompressed)) {
        fprintf(stderr, "Compression/decompression mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100);
    
    printf("\nCompressed data:\n");
    print_buffer_hex(&compressed);
    
    return EXIT_SUCCESS;
}