//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
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
    uint16_t algorithm;
    uint8_t checksum[16];
};

struct CompressionBlock {
    uint32_t block_id;
    uint32_t offset;
    uint32_t size;
    uint8_t flags;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint8_t *data_buffer;
    size_t buffer_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->version > 1) return 0;
    if (header->original_size > SIZE_MAX) return 0;
    if (header->compressed_size > SIZE_MAX) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int validate_block(const struct CompressionBlock *block, size_t max_offset) {
    if (block == NULL) return 0;
    if (block->offset > max_offset) return 0;
    if (block->size == 0 || block->size > 65536) return 0;
    if (block->flags > 7) return 0;
    return 1;
}

int initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
    memset(ctx->header.checksum, 0, sizeof(ctx->header.checksum));
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, uint32_t offset, uint32_t size, uint8_t flags) {
    if (ctx == NULL) return 0;
    if (size == 0 || size > 65536) return 0;
    if (flags > 7) return 0;
    
    uint32_t new_count = ctx->block_count + 1;
    if (new_count < ctx->block_count) return 0;
    
    struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_count * sizeof(struct CompressionBlock));
    if (new_blocks == NULL) return 0;
    
    ctx->blocks = new_blocks;
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = ctx->block_count;
    block->offset = offset;
    block->size = size;
    block->flags = flags;
    ctx->block_count = new_count;
    return 1;
}

int process_compression_data(struct CompressionContext *ctx, const uint8_t *data, size_t data_size) {
    if (ctx == NULL || data == NULL) return 0;
    if (data_size == 0 || data_size > SIZE_MAX / 2) return 0;
    
    uint8_t *new_buffer = realloc(ctx->data_buffer, data_size);
    if (new_buffer == NULL) return 0;
    
    ctx->data_buffer = new_buffer;
    memcpy(ctx->data_buffer, data, data_size);
    ctx->buffer_size = data_size;
    ctx->header.original_size = data_size;
    ctx->header.compressed_size = data_size / 2;
    
    for (size_t i = 0; i < sizeof(ctx->header.checksum); i++) {
        ctx->header.checksum[i] = (uint8_t)(data_size + i);
    }
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    free(ctx->blocks);
    free(ctx->data_buffer);
    memset(ctx, 0, sizeof(*ctx));
}

int main(void) {
    struct CompressionContext ctx;
    if (!initialize_context(&ctx)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t test_data[1024];
    for (size_t i = 0; i < sizeof(test_data); i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    if (!process_compression_data(&ctx, test_data, sizeof(test_data))) {
        fprintf(stderr, "Failed to process compression data\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    if (!add_compression_block(&ctx, 0, 256, 1)) {
        fprintf(stderr, "Failed to add compression block\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    if (!add_compression_block(&ctx, 256, 512, 2)) {
        fprintf(stderr, "Failed to add compression block\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    if (!add_compression_block(&ctx, 768, 256, 3)) {
        fprintf(stderr, "Failed to add compression block\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    if (!validate_header(&ctx.header)) {
        fprintf(stderr, "Invalid compression header\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    for (uint32_t i