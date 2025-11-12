//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint64_t original_size;
    uint64_t compressed_size;
    uint32_t checksum;
    uint8_t algorithm;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_id;
    uint32_t offset;
    uint32_t size;
    uint32_t compressed_size;
    uint8_t data[1024];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->version > 1) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    if (header->algorithm > 2) return 0;
    return 1;
}

int add_block(struct CompressionContext *ctx, uint32_t offset, uint32_t size, const uint8_t *data) {
    if (ctx == NULL || data == NULL) return 0;
    if (size > 1024) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = ctx->block_count;
    block->offset = offset;
    block->size = size;
    block->compressed_size = size;
    
    for (uint32_t i = 0; i < size; i++) {
        block->data[i] = data[i];
    }
    
    ctx->block_count++;
    return 1;
}

uint32_t simple_checksum(const uint8_t *data, size_t len) {
    if (data == NULL) return 0;
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum + data[i]) & 0xFFFFFFFF;
    }
    return sum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size > 104857600) return 0;
    
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.original_size = input_size;
    ctx->header.compressed_size = input_size;
    ctx->header.algorithm = 0;
    ctx->header.flags = 0;
    ctx->header.checksum = simple_checksum(input, input_size);
    
    size_t remaining = input_size;
    uint32_t offset = 0;
    
    while (remaining > 0) {
        uint32_t chunk_size = (remaining > 1024) ? 1024 : remaining;
        
        if (!add_block(ctx, offset, chunk_size, input + offset)) {
            return 0;
        }
        
        offset += chunk_size;
        remaining -= chunk_size;
    }
    
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, size_t output_size) {
    if (ctx == NULL || output == NULL) return 0;
    if (!validate_header(&ctx->header)) return 0;
    if (output_size < ctx->header.original_size) return 0;
    
    uint32_t total_size = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        const struct CompressionBlock *block = &ctx->blocks[i];
        if (block->offset + block->size > ctx->header.original_size) return 0;
        
        for (uint32_t j = 0; j < block->size; j++) {
            output[block->offset + j] = block->data[j];
        }
        total_size += block->size;
    }
    
    if (total_size != ctx->header.original_size) return 0;
    
    uint32_t calculated_checksum = simple_checksum(output, ctx->header.original_size);
    if (calculated_checksum != ctx->header.checksum) return 0;
    
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    ctx.max_blocks = 100;
    ctx.blocks = malloc(ctx.max_blocks * sizeof(struct CompressionBlock));
    if (ctx.blocks == NULL) {
        return 1;
    }
    ctx.block_count = 0;
    
    uint8_t test_data[2048];
    for (int i = 0; i < 2048; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    if (!compress_data(&ctx, test_data, 2048)) {
        free(ctx.blocks);
        return 1;
    }
    
    uint8_t decompressed[2048];
    if (!decompress_data(&ctx, decompressed, 2048)) {
        free(ctx.blocks);
        return 1;
    }
    
    int success = 1;
    for (int i = 0; i < 2048; i++) {