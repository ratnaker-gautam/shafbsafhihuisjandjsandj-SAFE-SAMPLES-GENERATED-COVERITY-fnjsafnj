//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressFunc)(const Buffer*);
typedef Buffer (*DecompressFunc)(const Buffer*);

Buffer null_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL) return output;
    
    output.size = input->size + 4;
    output.data = malloc(output.size);
    if (output.data == NULL) return output;
    
    uint32_t size_header = (uint32_t)input->size;
    output.data[0] = (size_header >> 24) & 0xFF;
    output.data[1] = (size_header >> 16) & 0xFF;
    output.data[2] = (size_header >> 8) & 0xFF;
    output.data[3] = size_header & 0xFF;
    
    memcpy(output.data + 4, input->data, input->size);
    return output;
}

Buffer null_decompress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size < 4) return output;
    
    uint32_t original_size = ((uint32_t)input->data[0] << 24) |
                            ((uint32_t)input->data[1] << 16) |
                            ((uint32_t)input->data[2] << 8) |
                            (uint32_t)input->data[3];
    
    if (original_size != input->size - 4) return output;
    
    output.size = original_size;
    output.data = malloc(output.size);
    if (output.data == NULL) return output;
    
    memcpy(output.data, input->data + 4, output.size);
    return output;
}

void free_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int compare_buffers(const Buffer *a, const Buffer *b) {
    if (a == NULL || b == NULL) return 0;
    if (a->size != b->size) return 0;
    return memcmp(a->data, b->data, a->size) == 0;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[256];
    printf("Enter text to compress (max 255 chars): ");
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    Buffer original = {NULL, 0};
    original.size = len;
    original.data = malloc(original.size);
    if (original.data == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    memcpy(original.data, input_str, original.size);
    
    printf("Original: %s (%zu bytes)\n", input_str, original.size);
    
    Buffer compressed = compress(&original);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        free_buffer(&original);
        return;
    }
    
    printf("Compressed: %zu bytes\n", compressed.size);
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free_buffer(&original);
        free_buffer(&compressed);
        return;
    }
    
    printf("Decompressed: ");
    fwrite(decompressed.data, 1, decompressed.size, stdout);
    printf(" (%zu bytes)\n", decompressed.size);
    
    if (compare_buffers(&original, &decompressed)) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    free_buffer(&original);
    free_buffer(&compressed);
    free_buffer(&decompressed);
}

int main() {
    CompressFunc compress_funcs[] = {null_compress};
    DecompressFunc decompress_funcs[] = {null_decompress};
    const char *names[] = {"Null compression"};
    int num_funcs = 1;
    
    printf("Available compression methods:\n");
    for (int i = 0; i < num_funcs; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (1-%d): ", num_funcs);
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    if (choice < 1 || choice > num_funcs) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_compression(compress_funcs[choice - 1], decompress_funcs[choice - 1]);
    
    return 0;
}