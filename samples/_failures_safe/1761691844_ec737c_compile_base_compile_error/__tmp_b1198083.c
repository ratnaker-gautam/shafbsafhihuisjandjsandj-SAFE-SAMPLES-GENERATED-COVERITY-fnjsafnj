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

static int compress_block(const struct CompressionBlock *block, struct CompressionContext *ctx, uint16_t *output, size_t *output_size) {
    if (block == NULL || ctx == NULL || output == NULL || output_size == NULL) return 0;
    if (block->size == 0 || block->size > MAX_BLOCK_SIZE) return 0;
    if (*output_size < block->size) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < block->size) {
        size_t best_match_len = 0;
        uint16_t best_code = 0;
        
        for (size_t i = 0; i < ctx->dict_size; i++) {
            size_t match_len = 0;
            while (match_len < ctx->dictionary[i].length && 
                   in_idx + match_len < block->size && 
                   match_len < 16) {
                if (block->data[in_idx + match_len] != ctx->dictionary[i].pattern[match_len]) {
                    break;
                }
                match_len++;
            }
            
            if (match_len > best_match_len && match_len == ctx->dictionary[i].length) {
                best_match_len = match_len;
                best_code = ctx->dictionary[i].code;
            }
        }
        
        if (best_match_len > 0) {
            if (out_idx >= *output_size) return 0;
            output[out_idx++] = best_code;
            in_idx += best_match_len;
        } else {
            if (out_idx >= *output_size) return 0;
            output[out_idx++] = (uint16_t)block->data[in_idx];
            in_idx++;
        }
    }
    
    *output_size = out_idx;
    ctx->compressed_size += out_idx * sizeof(uint16_t);
    return 1;
}

static int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size == 0 || block->size > MAX_BLOCK_SIZE) return 0;
    uint32_t computed_checksum = calculate_checksum(block->data, block->size);
    return computed_checksum == block->checksum;
}

int main(void) {
    struct CompressionContext ctx;
    if (!initialize_dictionary(&ctx)) {
        return 1;
    }
    
    struct CompressionBlock test_blocks[3];
    uint16_t compressed_data[MAX_BLOCK_SIZE * 2];
    size_t compressed_size;
    
    const char *test_strings[] = {
        "Hello, world! This is a test string for compression.",
        "Another test string with repeated patterns like abcabcabc.",
        "Short string."
    };
    
    for (int i = 0; i < 3; i++) {
        size_t len = strlen(test_strings[i]);
        if (len > MAX_BLOCK_SIZE) len = MAX_BLOCK_SIZE;
        
        memcpy(test_blocks[i].data, test_strings[i], len);
        test_blocks[i].size = len;
        test_blocks[i].checksum = calculate_checksum(test_blocks[i].data, test_blocks[i].size);
        
        if (!validate_block(&test_blocks[i])) {
            return 1;
        }
        
        compressed_size = sizeof(compressed_data) / sizeof(compressed_data[0]);
        if (!compress_block(&test_blocks[i], &ctx, compressed_data, &compressed_size)) {
            return 1;
        }
        
        ctx.total_blocks++;
        
        printf("Block %d: Original=%zu bytes, Compressed=%zu codes, Ratio=%.2f\n",
               i + 1, test_blocks[i].size, compressed_size, 
               (float)compressed_size * 2 / test_blocks[i].size);
    }
    
    printf("Total blocks processed: %