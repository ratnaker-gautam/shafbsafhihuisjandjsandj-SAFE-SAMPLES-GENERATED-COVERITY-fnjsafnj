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

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

Buffer compress_data(const char *input, size_t input_len) {
    Buffer output = create_buffer(MAX_OUTPUT_SIZE);
    if (output.data == NULL || !validate_input(input, input_len)) {
        return output;
    }
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input_len && out_pos < output.size - 2) {
        char current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 1) {
            output.data[out_pos++] = current;
            output.data[out_pos++] = (uint8_t)count;
        } else {
            output.data[out_pos++] = current;
        }
        
        i += count;
    }
    
    output.size = out_pos;
    return output;
}

Buffer decompress_data(const char *input, size_t input_len) {
    Buffer output = create_buffer(MAX_OUTPUT_SIZE);
    if (output.data == NULL || !validate_input(input, input_len)) {
        return output;
    }
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input_len && out_pos < output.size) {
        char current = input[i++];
        
        if (i < input_len && (unsigned char)input[i] > 1 && (unsigned char)input[i] <= 255) {
            size_t count = (unsigned char)input[i++];
            
            if (out_pos + count > output.size) {
                destroy_buffer(&output);
                return output;
            }
            
            for (size_t j = 0; j < count; j++) {
                output.data[out_pos++] = current;
            }
        } else {
            output.data[out_pos++] = current;
        }
    }
    
    output.size = out_pos;
    return output;
}

void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02x ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE];
    size_t input_len;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[--input_len] = '\0';
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    Buffer compressed = compress_data(input, input_len);
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data (hex): ");
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress_data((char*)compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        fprintf(stderr, "Decompression failed\n");
        destroy_buffer(&compressed);
        return 1;
    }
    
    printf("Decompressed: %s\n", decompressed.data);
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (decompressed.size == input_len && 
        memcmp(decompressed.data, input, input_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    destroy_buffer(&compressed);
    destroy_buffer(&decompressed);
    
    return 0;
}