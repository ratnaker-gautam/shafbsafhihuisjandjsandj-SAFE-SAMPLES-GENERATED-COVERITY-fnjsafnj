//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

Buffer recursive_compress(const uint8_t* data, size_t len, size_t pos) {
    Buffer result = {0};
    
    if (pos >= len) {
        return result;
    }
    
    uint8_t current = data[pos];
    size_t count = 1;
    
    size_t next_pos = pos + 1;
    while (next_pos < len && data[next_pos] == current && count < 255) {
        count++;
        next_pos++;
    }
    
    Buffer rest = recursive_compress(data, len, next_pos);
    
    if (count > 3 || !isprint(current)) {
        result.data[0] = 0xFF;
        result.data[1] = current;
        result.data[2] = (uint8_t)count;
        result.length = 3;
    } else {
        for (size_t i = 0; i < count; i++) {
            result.data[i] = current;
        }
        result.length = count;
    }
    
    if (result.length + rest.length <= MAX_INPUT_SIZE) {
        memcpy(result.data + result.length, rest.data, rest.length);
        result.length += rest.length;
    }
    
    return result;
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((uint8_t*)input, input_len, 0);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           input_len > 0 ? (100.0 * compressed.length / input_len) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.length; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}