//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    if (start >= end) {
        return result;
    }
    
    size_t count = 1;
    size_t i = start + 1;
    while (i < end && data[i] == data[start] && count < 255) {
        count++;
        i++;
    }
    
    if (count > 1) {
        result.data[0] = data[start];
        result.data[1] = (uint8_t)count;
        result.size = 2;
        
        Buffer rest = compress_chunk(data, start + count, end);
        if (result.size + rest.size <= MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    } else {
        result.data[0] = data[start];
        result.size = 1;
        
        Buffer rest = compress_chunk(data, start + 1, end);
        if (result.size + rest.size <= MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    }
    
    return result;
}

int validate_input(const char* input, size_t len) {
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void print_buffer(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        if (i > 0) printf(" ");
        printf("%02X", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        printf("Invalid input size\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk((uint8_t*)input, 0, len);
    
    printf("Original size: %zu\n", len);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(len - compressed.size) / len * 100.0f);
    printf("Compressed data: ");
    print_buffer(&compressed);
    
    return 0;
}