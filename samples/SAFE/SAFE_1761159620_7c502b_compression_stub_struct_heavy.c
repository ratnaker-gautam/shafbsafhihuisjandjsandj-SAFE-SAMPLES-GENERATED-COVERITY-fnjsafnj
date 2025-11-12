//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BLOCK_SIZE 1024
#define MAX_DICT_SIZE 256

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    uint32_t checksum;
};

struct DictionaryEntry {
    uint8_t pattern[16];
    size_t length;
    uint16_t code;
};

struct CompressionContext {
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
    uint32_t total_blocks;
    uint32_t compressed_size;
};

static uint32_t calculate_checksum(const uint8_t *data, size_t size) {
    uint32_t sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum = (sum + data[i]) & 0xFFFFFFFF;
    }
    return sum;
}

static int initialize_dictionary(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    
    ctx->dict_size = 0;
    ctx->total_blocks = 0;
    ctx->compressed_size = 0;
    
    for (size_t i = 0; i < 256; i++) {
        if (ctx->dict_size >= MAX_DICT_SIZE) break;
        ctx->dictionary[ctx->dict_size].pattern[0] = (uint8_t)i;
        ctx->dictionary[ctx->dict_size].length = 1;
        ctx->dictionary[ctx->dict_size].code = (uint16_t)ctx->dict_size;
        ctx->dict_size++;
    }
    
    return 1;
}

static int compress_block(struct CompressionContext *ctx, const struct CompressionBlock *block, uint8_t *output, size_t *output_size) {
    if (ctx == NULL || block == NULL || output == NULL || output_size == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE || *output_size < block->size * 2) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < block->size; ) {
        size_t best_match = 0;
        size_t best_length = 0;
        
        for (size_t j = 0; j < ctx->dict_size; j++) {
            size_t max_len = (block->size - i < ctx->dictionary[j].length) ? block->size - i : ctx->dictionary[j].length;
            if (max_len > best_length) {
                if (memcmp(&block->data[i], ctx->dictionary[j].pattern, max_len) == 0) {
                    best_match = j;
                    best_length = max_len;
                }
            }
        }
        
        if (best_length > 0) {
            if (out_idx + 2 >= *output_size) return 0;
            output[out_idx++] = (uint8_t)(ctx->dictionary[best_match].code >> 8);
            output[out_idx++] = (uint8_t)(ctx->dictionary[best_match].code & 0xFF);
            i += best_length;
        } else {
            if (out_idx + 1 >= *output_size) return 0;
            output[out_idx++] = block->data[i];
            i++;
        }
    }
    
    *output_size = out_idx;
    return 1;
}

static void process_user_input(void) {
    struct CompressionContext ctx;
    if (!initialize_dictionary(&ctx)) {
        printf("Failed to initialize compression context\n");
        return;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_BLOCK_SIZE - 1);
    
    char input_buffer[MAX_BLOCK_SIZE];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return;
    }
    
    struct CompressionBlock block;
    memcpy(block.data, input_buffer, input_len);
    block.size = input_len;
    block.checksum = calculate_checksum(block.data, block.size);
    
    uint8_t compressed[MAX_BLOCK_SIZE * 2];
    size_t compressed_size = sizeof(compressed);
    
    if (compress_block(&ctx, &block, compressed, &compressed_size)) {
        printf("Original size: %zu bytes\n", block.size);
        printf("Compressed size: %zu bytes\n", compressed_size);
        printf("Compression ratio: %.2f%%\n", 
               (block.size > 0) ? (100.0 * compressed_size / block.size) : 0.0);
        printf("Checksum: 0x%08X\n", block.checksum);
    } else {
        printf("Compression failed\n");
    }
}

int main(void) {
    process_user_input();
    return 0;
}