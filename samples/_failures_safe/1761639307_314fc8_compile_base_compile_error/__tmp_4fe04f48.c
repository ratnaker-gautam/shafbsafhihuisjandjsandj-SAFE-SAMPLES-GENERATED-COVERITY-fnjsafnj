//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BLOCK_SIZE 4096
#define MAX_DICT_SIZE 256

struct CompressionHeader {
    uint32_t magic;
    uint16_t version;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t algorithm;
    uint8_t checksum;
};

struct DictionaryEntry {
    uint8_t symbol;
    uint32_t frequency;
    uint16_t code;
    uint8_t code_length;
};

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    struct DictionaryEntry dict[MAX_DICT_SIZE];
    size_t dict_size;
    uint32_t crc;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    size_t total_blocks;
    uint32_t total_compressed;
    uint32_t total_original;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
}

uint32_t calculate_crc(const uint8_t *data, size_t len) {
    if (data == NULL && len > 0) return 0;
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return ~crc;
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE) return 0;
    if (block->dict_size > MAX_DICT_SIZE) return 0;
    
    uint32_t computed_crc = calculate_crc(block->data, block->size);
    if (computed_crc != block->crc) return 0;
    
    return 1;
}

int compress_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0 || input_size > MAX_BLOCK_SIZE) {
        return 0;
    }
    
    memset(&ctx->current_block, 0, sizeof(ctx->current_block));
    
    memcpy(ctx->current_block.data, input, input_size);
    ctx->current_block.size = input_size;
    
    uint32_t freq[256] = {0};
    for (size_t i = 0; i < input_size; i++) {
        if (freq[input[i]] < UINT32_MAX) {
            freq[input[i]]++;
        }
    }
    
    ctx->current_block.dict_size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0 && ctx->current_block.dict_size < MAX_DICT_SIZE) {
            ctx->current_block.dict[ctx->current_block.dict_size].symbol = (uint8_t)i;
            ctx->current_block.dict[ctx->current_block.dict_size].frequency = freq[i];
            ctx->current_block.dict[ctx->current_block.dict_size].code = (uint16_t)ctx->current_block.dict_size;
            ctx->current_block.dict[ctx->current_block.dict_size].code_length = 8;
            ctx->current_block.dict_size++;
        }
    }
    
    ctx->current_block.crc = calculate_crc(input, input_size);
    
    if (!validate_block(&ctx->current_block)) {
        return 0;
    }
    
    ctx->total_blocks++;
    if (ctx->total_original + input_size >= ctx->total_original) {
        ctx->total_original += (uint32_t)input_size;
    }
    uint32_t compressed_block_size = (uint32_t)(input_size + ctx->current_block.dict_size * 4);
    if (ctx->total_compressed + compressed_block_size >= ctx->total_compressed) {
        ctx->total_compressed += compressed_block_size;
    }
    
    return 1;
}

void update_header(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    ctx->header.original_size = ctx->total_original;
    ctx->header.compressed_size = ctx->total_compressed;
    ctx->header.checksum = (uint8_t)(calculate_crc((uint8_t*)&ctx->header, 12) & 0xFF);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[] = "This is a test string for compression demonstration.";
    size_t test_size = strlen((char*)test_data);
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %zu bytes\n", test_size);
    
    if (!compress_block(&ctx, test_data, test_size)) {
        printf("Compression failed\n");
        return 1;
    }
    
    update_header(&ctx);
    
    printf("Compression completed successfully\n");
    printf("Total blocks: %zu\n", ctx.total_