//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint32_t compressed_size;
    uint32_t original_size;
    uint8_t algorithm;
    uint8_t flags;
    uint16_t reserved;
};

struct CompressionBlock {
    uint32_t block_offset;
    uint32_t block_size;
    uint32_t compressed_block_size;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) {
        return 0;
    }
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        return 0;
    }
    
    memset(ctx->blocks, 0, max_blocks * sizeof(struct CompressionBlock));
    ctx->max_blocks = max_blocks;
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
    
    return 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->blocks != NULL) {
        for (uint32_t i = 0; i < ctx->block_count; i++) {
            if (ctx->blocks[i].data != NULL) {
                free(ctx->blocks[i].data);
            }
        }
        free(ctx->blocks);
    }
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t data_size) {
    if (ctx == NULL || data == NULL || data_size == 0 || data_size > 65536) {
        return 0;
    }
    
    if (ctx->block_count >= ctx->max_blocks) {
        return 0;
    }
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->data = malloc(data_size);
    if (block->data == NULL) {
        return 0;
    }
    
    memcpy(block->data, data, data_size);
    block->block_size = data_size;
    block->compressed_block_size = data_size;
    block->block_offset = ctx->header.compressed_size;
    
    ctx->header.compressed_size += data_size;
    ctx->header.original_size += data_size;
    ctx->block_count++;
    
    return 1;
}

int write_compressed_data(const struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL || strlen(filename) == 0) {
        return 0;
    }
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        const struct CompressionBlock *block = &ctx->blocks[i];
        if (fwrite(block->data, 1, block->compressed_block_size, file) != block->compressed_block_size) {
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 1;
}

int read_compressed_data(struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL || strlen(filename) == 0) {
        return 0;
    }
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    if (fread(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    if (ctx->header.magic != 0x43535442 || ctx->header.version != 1) {
        fclose(file);
        return 0;
    }
    
    if (ctx->header.compressed_size > 1048576) {
        fclose(file);
        return 0;
    }
    
    uint8_t *compressed_data = malloc(ctx->header.compressed_size);
    if (compressed_data == NULL) {
        fclose(file);
        return 0;
    }
    
    if (fread(compressed_data, 1, ctx->header.compressed_size, file) != ctx->header.compressed_size) {
        free(compressed_data);
        fclose(file);
        return 0;
    }
    
    fclose(file);
    
    ctx->block_count = 1;
    ctx->max_blocks = 1;
    ctx->blocks = malloc(sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        free(compressed_data);
        return 0;
    }
    
    memset(ctx->blocks, 0, sizeof(struct CompressionBlock));
    ctx->blocks[0].data = compressed_data;
    ctx->blocks[0].block_size = ctx->header.original_size;
    ctx->blocks[0].compressed_block_size = ctx->header.compressed_size;
    ctx->blocks[0].