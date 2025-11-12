//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t flags;
    uint8_t reserved;
};

struct CompressionBlock {
    uint32_t offset;
    uint32_t size;
    uint8_t checksum[16];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint8_t *input_data;
    uint32_t input_size;
    uint8_t *output_data;
    uint32_t output_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->original_size == 0) return 0;
    if (header->compressed_size == 0) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int validate_block(const struct CompressionBlock *block, uint32_t max_offset) {
    if (block == NULL) return 0;
    if (block->offset >= max_offset) return 0;
    if (block->size == 0) return 0;
    if (block->size > 65536) return 0;
    return 1;
}

int initialize_context(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    
    ctx->input_size = size;
    ctx->input_data = malloc(size);
    if (ctx->input_data == NULL) return 0;
    memcpy(ctx->input_data, data, size);
    
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = size;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.flags = 0;
    ctx->header.reserved = 0;
    
    ctx->block_count = (size + 4095) / 4096;
    if (ctx->block_count == 0) ctx->block_count = 1;
    
    ctx->blocks = malloc(ctx->block_count * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        free(ctx->input_data);
        return 0;
    }
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        ctx->blocks[i].offset = i * 4096;
        ctx->blocks[i].size = (i == ctx->block_count - 1) ? (size - ctx->blocks[i].offset) : 4096;
        if (ctx->blocks[i].size > 4096) ctx->blocks[i].size = 4096;
        memset(ctx->blocks[i].checksum, 0, 16);
    }
    
    ctx->output_size = 0;
    ctx->output_data = NULL;
    
    return 1;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || !validate_header(&ctx->header)) return 0;
    
    uint32_t estimated_size = ctx->input_size + sizeof(struct CompressionHeader) + 
                             ctx->block_count * sizeof(struct CompressionBlock);
    
    if (estimated_size < ctx->input_size) return 0;
    
    ctx->output_data = malloc(estimated_size);
    if (ctx->output_data == NULL) return 0;
    
    uint8_t *ptr = ctx->output_data;
    memcpy(ptr, &ctx->header, sizeof(struct CompressionHeader));
    ptr += sizeof(struct CompressionHeader);
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (!validate_block(&ctx->blocks[i], ctx->input_size)) {
            free(ctx->output_data);
            return 0;
        }
        memcpy(ptr, &ctx->blocks[i], sizeof(struct CompressionBlock));
        ptr += sizeof(struct CompressionBlock);
    }
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        uint32_t copy_size = ctx->blocks[i].size;
        if (ctx->blocks[i].offset + copy_size > ctx->input_size) {
            copy_size = ctx->input_size - ctx->blocks[i].offset;
        }
        memcpy(ptr, ctx->input_data + ctx->blocks[i].offset, copy_size);
        ptr += copy_size;
    }
    
    ctx->output_size = (uint32_t)(ptr - ctx->output_data);
    ctx->header.compressed_size = ctx->output_size;
    memcpy(ctx->output_data, &ctx->header, sizeof(struct CompressionHeader));
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->input_data != NULL) free(ctx->input_data);
    if (ctx->output_data != NULL) free(ctx->output_data);
    if (ctx->blocks != NULL) free(ctx->blocks);
}

int main(void) {
    uint8_t test_data[8192];
    for (int i = 0; i < 8192; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    struct CompressionContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    if