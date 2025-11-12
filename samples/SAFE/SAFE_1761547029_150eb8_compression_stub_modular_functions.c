//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t* data;
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

void destroy_buffer(Buffer* buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    if (size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer output = create_buffer(MAX_OUTPUT_SIZE);
    
    if (!validate_input(input, input_size) || output.data == NULL) {
        return output;
    }
    
    size_t output_pos = 0;
    
    for (size_t i = 0; i < input_size && output_pos < output.size - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (output_pos + 2 >= output.size) {
                break;
            }
            output.data[output_pos++] = 0xFF;
            output.data[output_pos++] = current;
            output.data[output_pos++] = (uint8_t)count;
            i += count;
        } else {
            if (output_pos + 1 >= output.size) {
                break;
            }
            output.data[output_pos++] = current;
            i++;
        }
    }
    
    output.size = output_pos;
    return output;
}

Buffer decompress_data(const uint8_t* input, size_t input_size) {
    Buffer output = create_buffer(MAX_OUTPUT_SIZE);
    
    if (!validate_input(input, input_size) || output.data == NULL) {
        return output;
    }
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size && output_pos < output.size) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output_pos + count > output.size) {
                break;
            }
            
            for (uint8_t j = 0; j < count && output_pos < output.size; j++) {
                output.data[output_pos++] = value;
            }
            
            i += 3;
        } else {
            output.data[output_pos++] = input[i++];
        }
    }
    
    output.size = output_pos;
    return output;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->data == NULL) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (buf->size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x44, 0x45, 0x45};
    size_t test_size = sizeof(test_data);
    
    printf("Original data (%zu bytes):\n", test_size);
    print_buffer_hex(&(Buffer){test_data, test_size});
    
    Buffer compressed = compress_data(test_data, test_size);
    if (compressed.data != NULL) {
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_buffer_hex(&compressed);
        
        Buffer decompressed = decompress_data(compressed.data, compressed.size);
        if (decompressed.data != NULL) {
            printf("Decompressed data (%zu bytes):\n", decompressed.size);
            print_buffer_hex(&decompressed);
            
            if (decompressed.size == test_size && 
                memcmp(decompressed.data, test_data, test_size) == 0) {
                printf("Compression/decompression successful!\n");
            } else {
                printf("Compression/decompression failed!\n");
            }
            
            destroy_buffer(&decompressed);
        }
        
        destroy_buffer(&compressed);
    }
    
    return 0;
}