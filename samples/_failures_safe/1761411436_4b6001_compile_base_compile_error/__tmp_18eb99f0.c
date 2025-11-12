//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm_id;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_offset;
    uint32_t block_size;
    uint16_t compression_type;
    uint8_t reserved[6];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
    uint8_t *input_data;
    uint32_t input_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    if (header->algorithm_id > 15) return 0;
    return 1;
}

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL) return 0;
    if (max_blocks == 0 || max_blocks > 10000) return 0;
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    
    ctx->max_blocks = max_blocks;
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm_id = 1;
    
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, uint32_t offset, uint32_t size, uint16_t type) {
    if (ctx == NULL) return 0;
    if (ctx->blocks == NULL) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    if (size == 0 || size > 65536) return 0;
    if (type > 7) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_offset = offset;
    block->block_size = size;
    block->compression_type = type;
    memset(block->reserved, 0, sizeof(block->reserved));
    
    ctx->block_count++;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    uint8_t checksum = 0;
    const uint8_t *header_bytes = (const uint8_t*)&ctx->header;
    for (size_t i = 0; i < sizeof(struct CompressionHeader) - 1; i++) {
        checksum ^= header_bytes[i];
    }
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        const uint8_t *block_bytes = (const uint8_t*)&ctx->blocks[i];
        for (size_t j = 0; j < sizeof(struct CompressionBlock); j++) {
            checksum ^= block_bytes[j];
        }
    }
    
    ctx->header.checksum = checksum;
}

int serialize_compression_data(const struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    if (ctx == NULL || output == NULL) return 0;
    if (!validate_header(&ctx->header)) return 0;
    
    uint32_t required_size = sizeof(struct CompressionHeader) + 
                            (ctx->block_count * sizeof(struct CompressionBlock));
    
    if (output_size < required_size) return 0;
    
    memcpy(output, &ctx->header, sizeof(struct CompressionHeader));
    
    uint8_t *current = output + sizeof(struct CompressionHeader);
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        memcpy(current, &ctx->blocks[i], sizeof(struct CompressionBlock));
        current += sizeof(struct CompressionBlock);
    }
    
    return 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    
    if (ctx->input_data != NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
    }
    
    ctx->block_count = 0;
    ctx->max_blocks = 0;
    ctx->input_size = 0;
}

int main(void) {
    struct CompressionContext ctx;
    
    if (!init_compression_context(&ctx, 10)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    ctx.header.original_size = 1024;
    ctx.header.compressed_size = 768;
    
    if (!add_compression_block(&ctx, 0, 256, 1)) {
        fprintf(stderr, "Failed to add compression block\n");
        cleanup_compression_context(&ctx);
        return 1;
    }
    
    if (!add_compression_block(&ctx, 256, 256, 2)) {
        fprintf(stderr, "Failed to add compression block\n");
        cleanup_compression_context(&ctx);
        return 1;
    }