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
    
    for (size_t i = 0; i < MAX_DICT_SIZE; i++) {
        ctx->dictionary[i].length = 0;
        ctx->dictionary[i].code = 0;
        memset(ctx->dictionary[i].pattern, 0, 16);
    }
    return 1;
}

static int compress_block(struct CompressionContext *ctx, const struct CompressionBlock *block, uint8_t *output, size_t *output_size) {
    if (ctx == NULL || block == NULL || output == NULL || output_size == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE || *output_size < block->size) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < block->size) {
        size_t best_match = 0;
        size_t best_length = 0;
        
        for (size_t i = 0; i < ctx->dict_size; i++) {
            if (ctx->dictionary[i].length == 0) continue;
            
            size_t match_len = 0;
            while (match_len < ctx->dictionary[i].length && 
                   in_idx + match_len < block->size && 
                   match_len < 16) {
                if (block->data[in_idx + match_len] != ctx->dictionary[i].pattern[match_len]) {
                    break;
                }
                match_len++;
            }
            
            if (match_len > best_length && match_len >= 2) {
                best_length = match_len;
                best_match = i;
            }
        }
        
        if (best_length >= 2) {
            if (out_idx + 2 <= *output_size) {
                output[out_idx++] = 0xFF;
                output[out_idx++] = (uint8_t)ctx->dictionary[best_match].code;
                in_idx += best_length;
            } else {
                return 0;
            }
        } else {
            if (out_idx + 1 <= *output_size) {
                output[out_idx++] = block->data[in_idx++];
            } else {
                return 0;
            }
        }
    }
    
    *output_size = out_idx;
    return 1;
}

static int add_to_dictionary(struct CompressionContext *ctx, const uint8_t *pattern, size_t length) {
    if (ctx == NULL || pattern == NULL || length == 0 || length > 16) return 0;
    if (ctx->dict_size >= MAX_DICT_SIZE) return 0;
    
    struct DictionaryEntry *entry = &ctx->dictionary[ctx->dict_size];
    memcpy(entry->pattern, pattern, length);
    entry->length = length;
    entry->code = (uint16_t)ctx->dict_size;
    ctx->dict_size++;
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    if (!initialize_dictionary(&ctx)) {
        return 1;
    }
    
    uint8_t common_patterns[][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {255, 255, 255, 255, 255, 255, 255, 255},
        {1, 2, 3, 4, 5, 6, 7, 8}
    };
    
    for (int i = 0; i < 3; i++) {
        if (!add_to_dictionary(&ctx, common_patterns[i], 8)) {
            return 1;
        }
    }
    
    struct CompressionBlock block;
    printf("Enter data to compress (max %d bytes): ", MAX_BLOCK_SIZE);
    
    int c;
    size_t input_size = 0;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_BLOCK_SIZE) {
        block.data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    block.size = input_size;
    block.checksum = calculate_checksum(block.data, block.size);
    
    uint8_t compressed[MAX_BLOCK_SIZE * 2];
    size_t compressed_size = sizeof(compressed);
    
    if (!compress_block(&ctx, &block,