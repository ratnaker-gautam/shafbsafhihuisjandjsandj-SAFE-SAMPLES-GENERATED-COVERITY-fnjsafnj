//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressFunc)(Buffer);
typedef Buffer (*DecompressFunc)(Buffer);

Buffer dummy_compress(Buffer input) {
    Buffer output;
    if (input.size == 0 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size + 4;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    output.data[0] = 'C';
    output.data[1] = 'M';
    output.data[2] = 'P';
    output.data[3] = 'R';
    memcpy(output.data + 4, input.data, input.size);
    
    return output;
}

Buffer dummy_decompress(Buffer input) {
    Buffer output;
    if (input.size < 4 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    if (input.data[0] != 'C' || input.data[1] != 'M' || input.data[2] != 'P' || input.data[3] != 'R') {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size - 4;
    if (output.size == 0) {
        output.data = NULL;
        return output;
    }
    
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input.data + 4, output.size);
    return output;
}

Buffer read_input(void) {
    Buffer buf;
    char input[MAX_INPUT_SIZE];
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        buf.data = NULL;
        buf.size = 0;
        return buf;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        buf.data = NULL;
        buf.size = 0;
        return buf;
    }
    
    buf.size = len;
    buf.data = malloc(len);
    if (buf.data == NULL) {
        buf.size = 0;
        return buf;
    }
    
    memcpy(buf.data, input, len);
    return buf;
}

void print_buffer(Buffer buf, const char *label) {
    printf("%s: ", label);
    if (buf.data == NULL || buf.size == 0) {
        printf("(empty)\n");
        return;
    }
    
    for (size_t i = 0; i < buf.size; i++) {
        if (buf.data[i] >= 32 && buf.data[i] <= 126) {
            putchar(buf.data[i]);
        } else {
            printf("[0x%02X]", buf.data[i]);
        }
    }
    printf("\n");
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer original = read_input();
    if (original.data == NULL || original.size == 0) {
        printf("No input received.\n");
        return;
    }
    
    printf("\nOriginal data:\n");
    print_buffer(original, "Input");
    
    Buffer compressed = compress(original);
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        free(original.data);
        return;
    }
    
    printf("\nAfter compression:\n");
    print_buffer(compressed, "Compressed");
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed.\n");
        free(original.data);
        free(compressed.data);
        return;
    }
    
    printf("\nAfter decompression:\n");
    print_buffer(decompressed, "Decompressed");
    
    int match = (original.size == decompressed.size) && 
                (memcmp(original.data, decompressed.data, original.size) == 0);
    printf("\nData integrity: %s\n", match ? "PASS" : "FAIL");
    
    free(original.data);
    free(compressed.data);
    free(decompressed.data);
}

int main(void) {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression/Decompression Demo\n");
    printf("=====================================\n\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}