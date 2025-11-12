//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

Buffer create_buffer(size_t size) {
    Buffer buf;
    buf.data = NULL;
    buf.size = 0;
    
    if (size == 0 || size > MAX_OUTPUT_SIZE) {
        return buf;
    }
    
    buf.data = malloc(size);
    if (buf.data != NULL) {
        buf.size = size;
    }
    return buf;
}

void destroy_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    if (size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

Buffer compress_rle(const uint8_t *input, size_t input_size) {
    Buffer output = create_buffer(MAX_OUTPUT_SIZE);
    if (output.data == NULL || !validate_input(input, input_size)) {
        return output;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size && out_index < output.size - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && 
               input[i + count] == current) {
            count++;
        }
        
        if (out_index + 2 >= output.size) {
            break;
        }
        
        output.data[out_index++] = (uint8_t)count;
        output.data[out_index++] = current;
        i += count;
    }
    
    output.size = out_index;
    return output;
}

Buffer decompress_rle(const uint8_t *input, size_t input_size) {
    Buffer output = create_buffer(MAX_OUTPUT_SIZE);
    if (output.data == NULL || !validate_input(input, input_size)) {
        return output;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_index < output.size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_index + count > output.size) {
            break;
        }
        
        for (uint8_t j = 0; j < count && out_index < output.size; j++) {
            output.data[out_index++] = value;
        }
    }
    
    output.size = out_index;
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
    if (buf->size > 0) {
        printf("\n");
    }
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x44, 0x44, 0x44, 0x44};
    size_t test_size = sizeof(test_data);
    
    printf("Original data (%zu bytes):\n", test_size);
    print_buffer_hex(&(Buffer){test_data, test_size});
    
    Buffer compressed = compress_rle(test_data, test_size);
    if (compressed.data != NULL) {
        printf("\nCompressed data (%zu bytes):\n", compressed.size);
        print_buffer_hex(&compressed);
        
        Buffer decompressed = decompress_rle(compressed.data, compressed.size);
        if (decompressed.data != NULL) {
            printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
            print_buffer_hex(&decompressed);
            
            if (decompressed.size == test_size && 
                memcmp(decompressed.data, test_data, test_size) == 0) {
                printf("\nCompression/Decompression successful!\n");
            } else {
                printf("\nCompression/Decompression failed!\n");
            }
            destroy_buffer(&decompressed);
        }
        destroy_buffer(&compressed);
    }
    
    return 0;
}