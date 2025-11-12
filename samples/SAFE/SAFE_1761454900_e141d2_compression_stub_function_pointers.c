//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(const Buffer*);

Buffer null_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL) return output;
    
    output.size = input->size * COMPRESSED_MULTIPLIER;
    output.data = malloc(output.size);
    if (output.data == NULL) return output;
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i * 2] = input->data[i];
        output.data[i * 2 + 1] = 0xFF;
    }
    
    return output;
}

Buffer run_length_encode(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) return output;
    
    output.size = input->size * 2;
    output.data = malloc(output.size);
    if (output.data == NULL) return output;
    
    size_t out_idx = 0;
    uint8_t current = input->data[0];
    uint8_t count = 1;
    
    for (size_t i = 1; i < input->size; i++) {
        if (input->data[i] == current && count < 255 && out_idx + 2 < output.size) {
            count++;
        } else {
            output.data[out_idx++] = count;
            output.data[out_idx++] = current;
            
            if (i < input->size) {
                current = input->data[i];
                count = 1;
            }
        }
    }
    
    if (out_idx + 2 <= output.size) {
        output.data[out_idx++] = count;
        output.data[out_idx++] = current;
    }
    
    output.size = out_idx;
    return output;
}

void print_buffer(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int get_user_input(Buffer *input) {
    if (input == NULL) return 0;
    
    char temp[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(temp, sizeof(temp), stdin) == NULL) return 0;
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    input->size = len;
    input->data = malloc(len);
    if (input->data == NULL) return 0;
    
    memcpy(input->data, temp, len);
    return 1;
}

int main(void) {
    Buffer input = {NULL, 0};
    CompressionFunc compressors[] = {null_compress, run_length_encode};
    const char *names[] = {"Null Compression", "Run-Length Encoding"};
    
    if (!get_user_input(&input)) {
        printf("Failed to get input or input empty.\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer(&input);
    
    for (int i = 0; i < 2; i++) {
        printf("\n%s:\n", names[i]);
        Buffer compressed = compressors[i](&input);
        
        if (compressed.data != NULL && compressed.size > 0) {
            printf("Compressed size: %zu bytes\n", compressed.size);
            print_buffer(&compressed);
            free(compressed.data);
        } else {
            printf("Compression failed\n");
        }
    }
    
    if (input.data != NULL) free(input.data);
    return 0;
}