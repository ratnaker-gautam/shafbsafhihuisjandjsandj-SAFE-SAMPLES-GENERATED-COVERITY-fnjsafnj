//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t reserved;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t data_offset;
    uint8_t compression_flags;
    uint8_t block_checksum;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
    uint8_t *input_data;
    uint32_t input_size;
};

struct CompressionResult {
    int success;
    uint32_t output_size;
    uint8_t *compressed_data;
    uint32_t compression_ratio;
};

void initialize_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->max_blocks = 16;
    ctx->blocks = malloc(ctx->max_blocks * sizeof(struct CompressionBlock));
    if (!ctx->blocks) {
        exit(EXIT_FAILURE);
    }
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx->blocks) {
        free(ctx->blocks);
    }
    if (ctx->input_data) {
        free(ctx->input_data);
    }
}

int validate_input_data(const uint8_t *data, uint32_t size) {
    if (!data || size == 0 || size > UINT32_MAX / 2) {
        return 0;
    }
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, uint32_t block_size, uint32_t data_offset) {
    if (ctx->block_count >= ctx->max_blocks) {
        uint32_t new_max = ctx->max_blocks * 2;
        if (new_max <= ctx->max_blocks) {
            return 0;
        }
        struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_max * sizeof(struct CompressionBlock));
        if (!new_blocks) {
            return 0;
        }
        ctx->blocks = new_blocks;
        ctx->max_blocks = new_max;
    }
    
    if (block_size == 0 || data_offset >= ctx->input_size) {
        return 0;
    }
    
    ctx->blocks[ctx->block_count].block_size = block_size;
    ctx->blocks[ctx->block_count].data_offset = data_offset;
    ctx->blocks[ctx->block_count].compression_flags = 0;
    ctx->blocks[ctx->block_count].block_checksum = 0;
    ctx->block_count++;
    return 1;
}

uint8_t calculate_checksum(const uint8_t *data, uint32_t size) {
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < size && i < UINT32_MAX; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

struct CompressionResult compress_data(struct CompressionContext *ctx) {
    struct CompressionResult result = {0};
    
    if (!ctx || !ctx->input_data || ctx->input_size == 0) {
        return result;
    }
    
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = ctx->input_size;
    ctx->header.algorithm = 1;
    ctx->header.checksum = calculate_checksum(ctx->input_data, ctx->input_size);
    
    uint32_t total_compressed = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data_offset + ctx->blocks[i].block_size > ctx->input_size) {
            return result;
        }
        total_compressed += ctx->blocks[i].block_size;
    }
    
    ctx->header.compressed_size = total_compressed;
    
    uint32_t output_size = sizeof(struct CompressionHeader) + 
                          ctx->block_count * sizeof(struct CompressionBlock) + 
                          total_compressed;
    
    if (output_size < total_compressed) {
        return result;
    }
    
    uint8_t *output = malloc(output_size);
    if (!output) {
        return result;
    }
    
    uint8_t *ptr = output;
    memcpy(ptr, &ctx->header, sizeof(struct CompressionHeader));
    ptr += sizeof(struct CompressionHeader);
    
    memcpy(ptr, ctx->blocks, ctx->block_count * sizeof(struct CompressionBlock));
    ptr += ctx->block_count * sizeof(struct CompressionBlock);
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        uint32_t offset = ctx->blocks[i].data_offset;
        uint32_t size = ctx->blocks[i].block_size;
        if (offset + size > ctx->input_size) {
            free(output);
            return result;
        }
        memcpy(ptr, ctx->input_data + offset, size);
        ptr += size;
    }
    
    result.success = 1;
    result.output_size = output_size;
    result.compressed_data = output;
    result.compression_ratio = (ctx->input_size * 100) / total_compressed;
    
    return result;
}

int main() {
    struct CompressionContext ctx;
    initialize_compression_context(&ctx);
    
    const char *test_data = "This is a sample text for compression